CREATE TABLE IF NOT EXISTS Entry (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name VARCHAR UNIQUE,
	body VARCHAR,
	created_at TIMESTAMP,
	updated_at TIMESTAMP,
	lock_revision INTEGER
);

CREATE INDEX IF NOT EXISTS EntryByName ON Entry (name);

CREATE TABLE IF NOT EXISTS User (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	username VARCHAR(128) UNIQUE,
	passhash VARCHAR, -- ideally would be BLOB, but BLOBs are not written to the db as intended
	salt VARCHAR,
	regist INTEGER
);

CREATE INDEX IF NOT EXISTS UserByName ON User (username);

CREATE TABLE IF NOT EXISTS EntryAudit (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at TIMESTAMP,
	entry_id INTEGER NOT NULL,
	done_by INTEGER NOT NULL,
	action_id INTEGER NOT NULL,
	oldtext VARCHAR,
	newtext VARCHAR,
	-- FOREIGN KEY (entry_id) REFERENCES Entry(id),
	FOREIGN KEY (done_by) REFERENCES User(id)
);
CREATE TABLE IF NOT EXISTS UserAudit (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	created_at TIMESTAMP,
	user INTEGER,
	done_by INTEGER NOT NULL,
	action_id INTEGER NOT NULL,
	oldtext VARCHAR,
	newtext VARCHAR
	-- FOREIGN KEY (user) REFERENCES User(id)
	-- FOREIGN KEY (done_by) REFERENCES User(id)
);
