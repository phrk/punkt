DROP SCHEMA IF EXISTS punkt CASCADE;
CREATE SCHEMA punkt;

CREATE TABLE punkt.places (id BIGSERIAL PRIMARY KEY,
							formatter_id BIGINT NOT NULL, 
						--	formatter_args TEXT NOT NULL,
							ownerid BIGINT NOT NULL,
							caption TEXT NOT NULL);

CREATE TABLE punkt.targets (id BIGSERIAL PRIMARY KEY,
							place_id BIGINT NOT NULL,
							flight_id BIGINT NOT NULL,
							formatter_args TEXT NOT NULL);
