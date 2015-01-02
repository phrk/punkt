#include "punktd.h"
#include "punktd_db.h"

PunktdDb::PunktdDb(PGPtr _pg):
	 m_pg(_pg){
	
}

PunktdDb::~PunktdDb() {
	
}

PunktPtr PunktdDb::getFreshPunkt() {
	
}

void Punktd::reloadDb() {
	
	hLockTicketPtr ticket = lock.lock();
	if (loadAds())
		loadPlaces();
}

bool Punktd::doCheckDbConn(size_t _attempt) {
	
	if (_attempt > 10)
		return false;
	
	if (PQstatus(m_pg) != CONNECTION_OK) {
		
		std::cout << "Reseting connection to PostgreSQL\n";
		PQreset(m_pg);
		return doCheckDbConn(_attempt + 1);
	}
	return true;
}

bool Punktd::checkDbConn() {
	
	return doCheckDbConn(0);
}

bool Punktd::loadAds() {
	
	std::string query("SELECT * FROM punkt.ads");
	PGresult *res = PQexec(m_pg, query.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cout << "Punktd::loadAds !PGRES_TUPLES_OK\n";
		
		checkDbConn();
		return false;
	}
	
	int ntuples = PQntuples(res);
	int ncols = PQnfields(res);
	
	uint64_t id;
	uint64_t format_id;
	std::string formatter_args;
	std::string targeter_args;
	uint64_t ad_owner;
	
	for (int t = 0; t<ntuples; t++) {	
		for (int c = 0; c < ncols; c++) {
		
			if (std::string("id") == PQfname(res, c)) {
				id = string_to_uint64( PQgetvalue(res, t, c) );
			}
			
			if (std::string("ad_owner") == PQfname(res, c)) {
				ad_owner = string_to_uint64( PQgetvalue(res, t, c) );
			}
			
			if (std::string("format_id") == PQfname(res, c)) {
				format_id = string_to_uint64( PQgetvalue(res, t, c) );
			}
			if (std::string("formatter_args") == PQfname(res, c)) {
				formatter_args = PQgetvalue(res, t, c);
			}
			
			if (std::string("targeter_args") == PQfname(res, c)) {
				targeter_args = PQgetvalue(res, t, c);
			}
		}
		//AdPtr ad (new Ad(id, format_id, ad_owner, formatter_args, targeter_args));
		
		m_targeter->updateAd(id, format_id, ad_owner, formatter_args, targeter_args);
	}
	return true;
}

bool Punktd::loadPlaceTarges(uint64_t _place, std::vector<uint64_t> &_ads_ids) {
	
	std::string query("SELECT ad_id FROM punkt.targets WHERE place_id=" + uint64_to_string(_place));
	PGresult *res = PQexec(m_pg, query.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cout << "Punktd::loadPlaceTarges !PGRES_TUPLES_OK _place: " << _place << std::endl;
		return false;
	}
	
	int ntuples = PQntuples(res);
	int ncols = PQnfields(res);
	
	for (int t = 0; t<ntuples; t++) {	
		for (int c = 0; c < ncols; c++) {
		
			if (std::string("ad_id") == PQfname(res, c))
				_ads_ids.push_back( string_to_uint64( PQgetvalue(res, t, c) ) );
		}
	}
	PQclear(res);
	return true;
}

void Punktd::loadPlaces() {

	std::string query  ("SELECT * FROM punkt.places");
	PGresult *res = PQexec(m_pg, query.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK) {
		std::cout << "Punktd::loadPlaces !PGRES_TUPLES_OK\n";
		return;
	}
	
	int ntuples = PQntuples(res);
	int ncols = PQnfields(res);
	
	uint64_t id;
	std::string targeter_args;
	//bool readok = false;
	
	for (int t = 0; t<ntuples; t++) {	
		//readok = false;
		for (int c = 0; c < ncols; c++) {
			if (std::string("id") == PQfname(res, c)) {
				id = string_to_uint64(PQgetvalue(res, t, c));
				//readok = true;
			}
			
			if (std::string("targeter_args") == PQfname(res, c)) {
				
				targeter_args = std::string(PQgetvalue(res, t, c));
			}
			
		}
		
//		if (!readok) {
//			std::cout << "Punktd::loadPlaces could not load place id\n";
//			continue;
//		}
		
		std::vector<uint64_t> ads_ids;
		if (loadPlaceTarges(id, ads_ids)) {
			m_targeter->updatePlace(id, targeter_args, ads_ids);
		}
	}
	
	PQclear(res);
}
