#include <string>
#include "Spell.cpp"

using namespace std;

struct Spellbook {
	string title;
	string author;
	int num_pages;
	int edition;
	int num_spells;
	float avg_success_rate;
	struct Spell* s;
};