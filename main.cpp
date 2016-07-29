#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include "vocab_card.h"
#include "dynamic_array.h"


using namespace std;

void AddVocab();
void Test();
void Edit();
void Display();
void ChangeFile();
void TestingNewArray();
void DeleteCard(string);
void ChangeFlashcard(string);
void SetExamInfo();
void Help();
void AboutMe();

Vocab_Card flashcard;



int main()
{
    string textfile;
    char answer;
    srand(time(0));
    textfile = flashcard.GetDefaultFile();
    textfile = textfile.replace(textfile.find(".txt"),4,"");

    int TextSize = 61 - (textfile.size());
    do
    {
        flashcard.ClearScreen();
        cout << right << setw(TextSize) << "You are currently studying for " << textfile << endl;

        cout << "Choose your option:" << endl
            << "A. Add new vocab word" << endl          //Make a Preset Chapter Option, so they don't have to type in the chapter for everything they do
            << "B. Test your knowledge" << endl        //If possible, cache the questions that are constantly being incorrect and use those for the harder difficulties
            << "C. Edit a Flashcard" << endl
            << "D. Display flash cards" << endl
            << "F. Configure your Default Class" << endl
            << "G. Change Exam Information" << endl
            << "H. Help Section" << endl
            << "I. About Me" << endl
            << "Q. Quit" << endl;

        cin >> answer;
        cin.ignore();
        switch (answer)
        {
        case 'A':
        case 'a':
            AddVocab();
            break;
        case 'B':
        case 'b':
            Test();
            break;

            break;
        case 'C':
        case 'c':
            Edit();             //UPDATE. SEEMS TO BE WORKING!! NEEDS MORE TESTING THOUGH!!!
            break;
        case 'D':
        case 'd':
            Display();
            break;
        case 'F':
        case 'f':
            ChangeFile();
            answer = 'Q';
            break;
        case 'G':
        case 'g':
            SetExamInfo();
            break;

        case 'H':
        case 'h':
            Help();
            break;

        case 'I':
        case 'i':
            AboutMe();
            break;
        case 'Q':
        case 'q':
            flashcard.Exit();
            break;            

        }

    } while (answer != 'q' && answer != 'Q');


    return 0;
}

void AddVocab()
{
    int chapter;
    int Chapter_Counter = flashcard.GetChapterCount() + 1;
    int New_Chapter = Chapter_Counter;

    string Vocab_Word, answer;
    Dynamic_Array<string> definitions;

    do {
        cout << "What Chapter did you want to add the vocab word to?" << endl;
        flashcard.ShowChapters();
        cout << New_Chapter << ". New Chapter" << endl;
        cin >> chapter;
        cin.ignore();

    } while (chapter < 0 || chapter > Chapter_Counter);


    do {
        answer = "0";
        cout << "Vocabulary Word: ";
        getline(cin, Vocab_Word);
        cout << "Enter your definitions. Press enter to put in a new definition. "
            << endl << "Type \"t\" and Enter on a new line when you are finished with the notecard and type "
            << endl << "\"e\" and Enter on a new line when you are done adding cards to the chapter" << endl << endl;
        for (int k = 0; answer != "t"; k++)
        {
            getline(cin, answer);
            if (answer == "t" || answer == "e")
            {
                break;
            }

            definitions.push_back(answer);
        }

        flashcard.AddCard(chapter, Vocab_Word, definitions);


        for (int i = 0, count = definitions.size(); i < count; i++)
        {
            definitions.pop_back();
        }

        if (chapter == New_Chapter)
            chapter = flashcard.GetChapterCount();

    } while (answer == "t");

}

void Display()
{
    flashcard.ClearScreen();
    int chapter;
    int Chapter_Counter = flashcard.GetChapterCount() + flashcard.GetExamCount() + 1;
    int All_Chapters = Chapter_Counter;
    do {
        cout << "What Chapters did you want to display?" << endl;
        flashcard.ShowChapters();

        for (int i = 0, ExamCount = flashcard.GetExamCount(), OptionNumber = flashcard.GetChapterCount(); i < ExamCount; i++)
            cout << OptionNumber + (i + 1) << ". Exam " << i + 1 << " material" << endl;

        cout << All_Chapters << ". All Chapters" << endl;
        cin >> chapter;
        cin.ignore();

    } while (chapter < 0 || chapter > Chapter_Counter);

    flashcard.Display(chapter);


    flashcard.PauseProgram();

}

void Test()
{
    flashcard.ClearScreen();
    int value, answer;
    int Chapter_counter = flashcard.GetChapterCount() + flashcard.GetExamCount() + 1;
    int All_chapters = Chapter_counter;

    do {
        cout << "What chapter did you want to study for?" << endl;
        flashcard.ShowChapters();

        for (int i = 0, ExamCount = flashcard.GetExamCount(), OptionNumber = flashcard.GetChapterCount(); i < ExamCount; i++)
            cout << OptionNumber + (i + 1) << ". Exam " << i + 1 << " material" << endl;

        cout << All_chapters <<". All Chapters" << endl;
        cin >> value;
        cin.ignore();

    } while (value < 0 || value > Chapter_counter);


    flashcard.ClearScreen();

    cout << "Which Game did you want to play?" << endl
         << "1. Normal Randomized Game" << endl
         << "2. Multiple Choice Game" << endl << endl;

    do{
    cin >> answer;
    cin.ignore();
    }while(answer != 1 && answer != 2);

    switch(answer)
    {
        case 1:
            flashcard.TemporaryTest(value);
            break;
        case 2:
            flashcard.MultipleChoiceGame(value);
            break;
    }

}



void Edit()
{
    string answer;
    string value;
    int option;

    cout << "What is the card you are looking for?" << endl;
    getline(cin, answer);
    value = flashcard.EditCard(answer);

    bool CardFound = flashcard.CardFound();

    if(flashcard.GetSearchSize() == 0 && value == "NULL")
    {
        cout << "Sorry, the flashcard was not found";
        flashcard.PauseProgram();
        return;
    }

    else if (CardFound == true)
    {
        CardFound = false;
        //cout << "We found the card, what did you want to do with it?";
        answer = value;
    }
    else
    {

        int quit = flashcard.GetSearchSize()+ 1;
        cout << "Which one did you want to choose from?" << endl;

        flashcard.ShowSearch();
        cout << quit << ". Quit" << endl;

        cin >> option;
        cin.ignore();
            if (option == quit)
               return;
            else
                answer = flashcard.GetSearchValue(option-1);

    }
    flashcard.ShowFlashCard(answer);
    cout << "What did you want to do with the card?" << endl;

    cout << "1. Delete Flashcard" << endl
         << "2. Edit Flashcard" << endl
         << "3. Quit" << endl;

    cin >> option;
    cin.ignore();
    switch(option)
    {
        case 1:
            DeleteCard(answer);
            break;
        case 2:
            ChangeFlashcard(answer);
            break;
        case 3:
            break;

    }


}


void ChangeFile() {

    int value;
    string TextName;
    int NewTextOption, deletefile;
    bool fileChanged = false;
    char answer;


  do{
        cout << "Which option did you want to choose from?" << endl;
        flashcard.ShowTextFiles();
        NewTextOption = flashcard.GetTextFileSize() + 1;
        deletefile = NewTextOption+1;
        cout <<  NewTextOption << ". Add a new class to add flashcards to" << endl;
        cout << deletefile << ". Delete a class of flashcards" << endl;
        cin >> value;
        cin.ignore();

    }while(value < 1 && value <= flashcard.GetTextFileSize()+2);

    if (value == deletefile)
    {
        flashcard.ClearScreen();
        flashcard.ShowTextFiles();
        do{
            cout << "Which file do you wish to delete?" << endl;
            cin >> value;
            cin.ignore();
          }while (value < 0 && value > flashcard.GetTextFileSize());


        flashcard.DeleteTextFile(value);

    }
    else if (value == NewTextOption)
    {
        cout << endl << "What do you want the new class to be called?" << endl;
        getline(cin, TextName);
        flashcard.SetExamToFile(TextName);

        cout << endl <<  "The file has been added sucessfully!!" << endl;
        do{
            cout << "Did you want to make this your default card? (Y/N)" << endl;
            cin >> answer;
            cin.ignore();
        }while(answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');

        if (answer == 'Y' || answer == 'y')
           {
            flashcard.ClearScreen();
            flashcard.SetDefaultFile(TextName);
            }


    }
    else
        fileChanged = flashcard.ChangeDefaultTextFile(value);

    flashcard.ClearScreen();

        cout << "The file has successfully changed" << endl;

    cout << "Please reopen the program when it closes" << endl;
    flashcard.PauseProgram();
    return;
}

void ChangeFlashcard(string answer)
{

    string value;
    int definitionCount = 0;

    int input = 0, choice;



do{
        flashcard.ClearScreen();
        flashcard.ShowFlashCard(answer);
    do{
        cout << "What would you like to edit?"<< endl;

        cout << "1. Change the vocab word name" << endl
             << "2. Change the definitions" << endl
             << "3. Quit" << endl;

        cin >> input;
        cin.ignore();

    }while(input > 3 || input < 1);

    switch (input) {
    case 1:
        cout << endl <<  endl << "What did you want to change the name to?" << endl;
        cin >> value;
        cin.ignore();
        flashcard.SetVocabWord(answer, value);
        answer = value;
        break;
     case 2:
       do{
        cout << "Which definition number did you want to change?" << endl;
         flashcard.ShowDefinitions(answer, definitionCount);
         cin >> choice;
         cin.ignore();

        }while(choice < 0 || choice > definitionCount);

        flashcard.ShowFlashCard(answer);
        cout << "What did you want to change it to?" << endl;
        getline(cin, value);
        flashcard.SetDefinition(answer, choice, value);
        break;

     case 3:
        break;

    default:
        break;
    }
        }while(input != 3);


}

void DeleteCard(string answer)
{
    flashcard.Delete(answer);
}


void Help()
{
    char answer = 'P';


 while(answer != 'Q')
 {
    do{
    flashcard.ClearScreen();
    cout << "What section were you confused about?" << endl << endl
         << "A. Add new vocab word" << endl
         << "B. Test your knowledge" << endl
         << "C. Edit a Flashcard" << endl
         << "D. Display flash cards" << endl
         << "E. Configure your Default Class" << endl
         << "F. Change Exam Information" << endl
         << "G. Where are my class files located?" << endl << endl
         << "Q. Quit to the main menu" << endl;
    cin >> answer;
    cin.ignore();

    if (islower(answer))
        answer = answer - 32;

    }while(answer < 'A' || answer > 'Q');

    if (answer > 'G')
        break;
    else
        flashcard.Help(answer);

    flashcard.PauseProgram();
  }

}

void AboutMe()
{

    flashcard.AboutMe();
    flashcard.PauseProgram();
}

void SetExamInfo()
{
    char answer;
        flashcard.ClearScreen();
        cout << "This is your current setup:" << endl << endl;

        flashcard.ShowExamInfo();

        do{
             cout << "Did you want to change this? (Y/N)" << endl;
                    cin >> answer;
             cin.ignore();
        } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');

        if (answer == 'N' || answer == 'n')
            return;

        flashcard.SetExamInfo();
}
