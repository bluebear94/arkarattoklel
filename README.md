## rattoarkaklel

Source code for what I hope will be the Arka-English dictionary.

### How to build

Install [TreeFrog](http://www.treefrogframework.org/), then type in

    qmake -r "CONFIG+=debug" # for a debug build
    qmake -r "CONFIG+=release" # for a release build

Then initialise the database:

    mkdir -p db
    sqlite3 db/klel < sql/init.sql

(This assumes that you're using SQLite. If you want to use another DBMS,
then you're unfortunately on your own.)

Then use `treefrog -e dev` to start the dev server.

(Steps based on [this page](http://www.treefrogframework.org/en/user-guide/tutorial/).)

### TODOs

* authentication in order to avoid spam and/or vandalism
* link to entries in Arka-Japanese dictionary
* pull list of entries in Arka-Japanese dictionary to compare and list entries
  that should be translated next
