CREATE TABLE IF NOT EXISTS Entry (
	id INTEGER PRIMARY KEY AUTOINCREMENT,
	name VARCHAR UNIQUE,
	body VARCHAR,
	created_at TIMESTAMP,
	updated_at TIMESTAMP,
	lock_revision INTEGER
);

CREATE INDEX IF NOT EXISTS EntryByName ON Entry (Name);
