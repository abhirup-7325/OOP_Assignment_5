/*
Each cricketer has name, date of birth and matches played. Cricketer may be a
bowler or batsman. For a bowler, number of wickets taken, average economy is
stored. For a batsman, total runs scored, average score is stored. A double
wicket pair is formed taking a bowler and a batsman. An all-rounder is both a
bowler and batsman. Support must be there to show the details of a cricketer,
bowler, batsmen, all-rounder and the pair.
Design the classes and implement.
*/

#include <iostream>
#include <string>
using namespace std;

class Cricketer {
protected:
    string name;
    string date_of_birth;
    int matches_played;

public:
    Cricketer(string name, string date_of_birth, int matches_played)
        : name(name), date_of_birth(date_of_birth), matches_played(matches_played) {}

    virtual void show_details() const {
        cout << "Name: " << name << endl;
        cout << "Date of Birth: " << date_of_birth << endl;
        cout << "Matches Played: " << matches_played << endl;
    }

    virtual ~Cricketer() {}
};

class Bowler : virtual public Cricketer {
protected:
    int wickets_taken;
    double average_economy;

public:
    Bowler(string name, string date_of_birth, int matches_played, int wickets_taken, double average_economy)
        : Cricketer(name, date_of_birth, matches_played), wickets_taken(wickets_taken), average_economy(average_economy) {}

    void show_details() const override {
        Cricketer::show_details();
        cout << "Wickets Taken: " << wickets_taken << endl;
        cout << "Average Economy: " << average_economy << endl;
    }
};

class Batsman : virtual public Cricketer {
protected:
    int total_runs;
    double average_score;

public:
    Batsman(string name, string date_of_birth, int matches_played, int total_runs, double average_score)
        : Cricketer(name, date_of_birth, matches_played), total_runs(total_runs), average_score(average_score) {}

    void show_details() const override {
        Cricketer::show_details();
        cout << "Total Runs: " << total_runs << endl;
        cout << "Average Score: " << average_score << endl;
    }
};

class AllRounder : public Bowler, public Batsman {
public:
    AllRounder(string name, string date_of_birth, int matches_played, int wickets_taken, double average_economy,
               int total_runs, double average_score)
        : Cricketer(name, date_of_birth, matches_played), Bowler(name, date_of_birth, matches_played, wickets_taken, average_economy),
          Batsman(name, date_of_birth, matches_played, total_runs, average_score) {}

    void show_details() const override {
        Cricketer::show_details();
        cout << "Wickets Taken: " << Bowler::wickets_taken << endl;
        cout << "Average Economy: " << Bowler::average_economy << endl;
        cout << "Total Runs: " << Batsman::total_runs << endl;
        cout << "Average Score: " << Batsman::average_score << endl;
    }
};

class DoubleWicketPair {
private:
    Bowler* bowler;
    Batsman* batsman;

public:
    DoubleWicketPair(Bowler* bowler, Batsman* batsman) : bowler(bowler), batsman(batsman) {}

    void show_details() const {
        cout << "Double Wicket Pair: " << endl;
        cout << "Bowler Details:" << endl;
        bowler->show_details();
        cout << "Batsman Details:" << endl;
        batsman->show_details();
    }
};

class System {
private:
    Bowler* bowler;
    Batsman* batsman;
    AllRounder* allRounder;
    DoubleWicketPair* doubleWicketPair;

public:
    System() : bowler(nullptr), batsman(nullptr), allRounder(nullptr), doubleWicketPair(nullptr) {}

    void show_menu() {
        int choice;
        do {
            cout << "\nCricketer System Menu:\n";
            cout << "1. Add Bowler\n";
            cout << "2. Add Batsman\n";
            cout << "3. Add All-rounder\n";
            cout << "4. Create Double Wicket Pair\n";
            cout << "5. Show Bowler Details\n";
            cout << "6. Show Batsman Details\n";
            cout << "7. Show All-rounder Details\n";
            cout << "8. Show Double Wicket Pair Details\n";
            cout << "9. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: add_bowler(); break;
                case 2: add_batsman(); break;
                case 3: add_allrounder(); break;
                case 4: create_double_wicket_pair(); break;
                case 5: show_bowler_details(); break;
                case 6: show_batsman_details(); break;
                case 7: show_allrounder_details(); break;
                case 8: show_double_wicket_pair_details(); break;
                case 9: cout << "Exiting...\n"; break;
                default: cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 9);
    }

private:
    void add_bowler() {
        string name, date_of_birth;
        int matches_played, wickets_taken;
        double average_economy;
        cout << "Enter Bowler's Name: ";
        cin >> name;
        cout << "Enter Bowler's Date of Birth (YYYY-MM-DD): ";
        cin >> date_of_birth;
        cout << "Enter Matches Played: ";
        cin >> matches_played;
        cout << "Enter Wickets Taken: ";
        cin >> wickets_taken;
        cout << "Enter Average Economy: ";
        cin >> average_economy;
        bowler = new Bowler(name, date_of_birth, matches_played, wickets_taken, average_economy);
        cout << "Bowler added successfully.\n";
    }

    void add_batsman() {
        string name, date_of_birth;
        int matches_played, total_runs;
        double average_score;
        cout << "Enter Batsman's Name: ";
        cin >> name;
        cout << "Enter Batsman's Date of Birth (YYYY-MM-DD): ";
        cin >> date_of_birth;
        cout << "Enter Matches Played: ";
        cin >> matches_played;
        cout << "Enter Total Runs: ";
        cin >> total_runs;
        cout << "Enter Average Score: ";
        cin >> average_score;
        batsman = new Batsman(name, date_of_birth, matches_played, total_runs, average_score);
        cout << "Batsman added successfully.\n";
    }

    void add_allrounder() {
        string name, date_of_birth;
        int matches_played, wickets_taken, total_runs;
        double average_economy, average_score;
        cout << "Enter All-rounder's Name: ";
        cin >> name;
        cout << "Enter All-rounder's Date of Birth (YYYY-MM-DD): ";
        cin >> date_of_birth;
        cout << "Enter Matches Played: ";
        cin >> matches_played;
        cout << "Enter Wickets Taken: ";
        cin >> wickets_taken;
        cout << "Enter Average Economy: ";
        cin >> average_economy;
        cout << "Enter Total Runs: ";
        cin >> total_runs;
        cout << "Enter Average Score: ";
        cin >> average_score;
        allRounder = new AllRounder(name, date_of_birth, matches_played, wickets_taken, average_economy, total_runs, average_score);
        cout << "All-rounder added successfully.\n";
    }

    void create_double_wicket_pair() {
        if (bowler && batsman) {
            doubleWicketPair = new DoubleWicketPair(bowler, batsman);
            cout << "Double Wicket Pair created successfully.\n";
        } else {
            cout << "Bowler and Batsman must be added first.\n";
        }
    }

    void show_bowler_details() const {
        if (bowler) {
            bowler->show_details();
        } else {
            cout << "No bowler found.\n";
        }
    }

    void show_batsman_details() const {
        if (batsman) {
            batsman->show_details();
        } else {
            cout << "No batsman found.\n";
        }
    }

    void show_allrounder_details() const {
        if (allRounder) {
            allRounder->show_details();
        } else {
            cout << "No all-rounder found.\n";
        }
    }

    void show_double_wicket_pair_details() const {
        if (doubleWicketPair) {
            doubleWicketPair->show_details();
        } else {
            cout << "No double wicket pair found.\n";
        }
    }
};