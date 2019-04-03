#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>

#include "Wizard.cpp"
#include "Spellbook.cpp"
#include "Spell.cpp"

using namespace std;

Wizard* CurrentWizard;
Wizard& Wizards;

int main(std::string fileName1, std::string fileName2)
{

	//Assume files are in correct order?
	if (attemptParseOfFiles(fileName1, fileName2) != true)
	{
		printf("Please ensure the file names are spelled correctly or that the data has not been corrupted.");
		return -1;
	}


	//At this point, we should have 2 lists of data, correctly parsed from the two files
	//We can now start the loop

	int numberOfFailedOperations = 0;
	bool loggedIn = AttemptLogin();

	while (loggedIn == true && numberOfFailedOperations < 3) //TODO Make 3 a const
	{

		//	AskOption()
		//		switch (choice)
		//		{
		//		case 1:
		//		case 2:
		//		case 3:
		//		case 4:
		//		default:
		//			numberOfFailedOperations++
		//		}

		//	//Only should move on to this step if the option was valid

		//	AskDisplay()
		//		switch (choice)
		//		{
		//		case 1:
		//			print
		//		case 2:
		//			AttemptWriteData()
		//		default:
		//			numberOfFailedOperations++
		//		}
		//}
	}

	//Free up memory on exit

	return 0;
}

bool attemptParseOfFiles(std::string wizardDataFile, std::string spellbookDataFile)
{
	bool wizardParse_WasSuccessful = ReadInWizards(wizardDataFile);
	bool spellBookParse_WasSuccessful = ReadInSpellBooks(spellbookDataFile);

	if (wizardParse_WasSuccessful && spellBookParse_WasSuccessful)
	{
		return true;
	}
	return false;
}

bool ReadInWizards(string wizardDataFile)
{
	std::ifstream inputFileStream;

	try
	{
		inputFileStream.open(wizardDataFile, std::ifstream::in);

		string line;
		getline(inputFileStream, line);
		int numberOfWizards = stoi(line);

		//While not at end of file, continue reading in lines
		do
		{

			string name;
			string idAsString;
			string password;
			string title;
			string beardLengthAsString;

			/*
				The assumption is that the file is formatted as such:
				<wizard name> <wizard ID#> <wizard username> <wizard position title> <wizard beard length>

				In this case, we can use:
					getline(inputFileStream, <string>, ' ')

				to handle parsing each block of text:
					name = "<wizard name>"

				and then after reading in the whole line (all of the data for that instance of Wizard, we can
				create a new Wizard instance and start populating the fields.
			*/

			getline(inputFileStream, name, ' ');
			getline(inputFileStream, idAsString, ' ');
			getline(inputFileStream, password, ' ');
			getline(inputFileStream, title, ' ');
			getline(inputFileStream, beardLengthAsString);

			Wizard newWizard = Wizard();

			newWizard.name = name;
			newWizard.password = password;
			newWizard.position_title = title;

			/*
				The only exception is for the ID and Beard Length fields, as these are specifically not strings,
				so we use the built-in functionality to parse strings into the corresponding data types; and assign
				the result to the instance.
			*/

			newWizard.id = stoi(idAsString);
			newWizard.beard_length = stof(beardLengthAsString);

			/*
				For the most part, we're trusting the file provided is correct and the data is specifically in this order.

				If the parsing to int and float fails, an exception should (ideally) be thrown, and we'll use this opportunity
				to close the application; as the data file is not correct and we don't have anyway of interpreting what part is
				incorrect.
			*/


		} while (inputFileStream.eof() == false);


		inputFileStream.close();
		return true;
	}
	catch (const std::exception & e)
	{
		/*
			This means either the data file was corrupted or doesn't exist.

			There's no point in continuing to execute the program at this stage.
		*/
		std::cout << "An exception was thrown while attempting to parse '" << wizardDataFile << "': " << e.what() << "\n";
		return false;
	}
}

bool ReadInSpellBooks(string spellbookDataFile)
{
	std::ifstream inputFileStream;

	try
	{
		inputFileStream.open(spellbookDataFile, std::ifstream::in);

		string line;
		getline(inputFileStream, line);
		int numberOfSpellbooks = stoi(line);

		//While not at end of file, continue reading in lines
		do
		{
			/*
				Similiarly to ReadInWizards:

				The assumption is that the file is formatted as such:
				<title of first spellbook> <author> <number of pages> <edition> <number of spells in book>
				<title of spell> <success rate> <effect>
				<title of spell> <success rate> <effect>
				<title of spell> <success rate> <effect>
				...

				In this case, we can still use the same general approach to parse the Spellbook's data.
				Once we've made the spellbook instance this time; that will tell us how many lines to read for
				constructing the spells.
			*/
			string titleOfBook;
			string author;
			string numberOfPagesAsString;
			string editionAsString;
			string numberOfSpellsAsString;

			getline(inputFileStream, titleOfBook, ' ');
			getline(inputFileStream, author, ' ');
			getline(inputFileStream, numberOfPagesAsString, ' ');
			getline(inputFileStream, editionAsString, ' ');
			getline(inputFileStream, numberOfSpellsAsString);

			Spellbook newSpellBook = Spellbook();

			newSpellBook.title = titleOfBook;
			newSpellBook.author = author;
			newSpellBook.num_pages = stoi(numberOfPagesAsString);
			newSpellBook.edition = stoi(editionAsString);
			newSpellBook.num_spells = stoi(numberOfSpellsAsString);

			/*
				Now that we have our spellbook, lets read in the spells:
			*/
			float sumOfSpellSuccessRates = 0.0f;
			for (int i = 0; i < newSpellBook.num_spells; i++)
			{
				string titleOfSpell;
				string successRateAsString;
				string effect;


				getline(inputFileStream, titleOfSpell, ' ');
				getline(inputFileStream, successRateAsString, ' ');
				getline(inputFileStream, effect);

				Spell newSpell = Spell();

				newSpell.name = titleOfSpell;
				newSpell.success_rate = stof(successRateAsString);
				newSpell.effect = effect;

				//TODO:: Add spell to newSpellbook

				sumOfSpellSuccessRates += newSpell.success_rate;
			}

			newSpellBook.avg_success_rate = sumOfSpellSuccessRates / newSpellBook.num_spells;

		} while (inputFileStream.eof() == false);


		inputFileStream.close();
		return true;
	}
	catch (const std::exception & e)
	{
		/*
			This means either the data file was corrupted or doesn't exist.

			There's no point in continuing to execute the program at this stage.
		*/
		std::cout << "An exception was thrown while attempting to parse '" << spellbookDataFile << "': " << e.what() << "\n";
		return false;
	}
}

bool AttemptLogin()
{
	int numberOfFailedOperations = 0;
	do
	{
		string user = "";
		string password = "";

		printf("Please enter your id:");
		cin >> user;
		printf("Please enter your password:");
		cin >> password;

		bool correctUsername = false;
		bool correctPassword = false;

		for (int i = 0; i < sizeof(Wizards); i++)
		{
			//TODO Refactor to actually compare name and password
			if ((&Wizards)[i].name == user && (&Wizards)[i].password == password)
			{
				CurrentWizard = &((&Wizards)[i]);

				//Print Welcome message for successful login
				cout << "Welcome " + CurrentWizard->name;
				cout << "ID: " + CurrentWizard->id;
				cout << "Status: " + CurrentWizard->position_title;
				cout << "Beard Length: " + to_string(CurrentWizard->beard_length);

				return true;
			}
		}

		printf("Invalid username or password.\n");
		numberOfFailedOperations++;
	} while (numberOfFailedOperations < 3); //TODO Make 3 a const


	return false;
}