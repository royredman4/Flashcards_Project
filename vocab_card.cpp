#include "vocab_card.h"

Vocab_Card::Vocab_Card()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
    OS_Name = "Windows";

    char t[MAX_PATH];
    SHGetFolderPathA(NULL, CSIDL_PROFILE, NULL, 0, t);
    string temp = t;
    FolderLocation = temp + "\\Documents\\Vocabulary_Program\\";

    system("if not exist \"C:\\Users\\%USERNAME%\\Documents\\Vocabulary_Program\" mkdir \"C:\\Users\\%USERNAME%\\Documents\\Vocabulary_Program\"");

#elif __linux__
    OS_Name = "Linux";
    FolderLocation = "/home/";
    string username = getpwuid(getuid())->pw_name;
    FolderLocation = FolderLocation + username + "/Vocabulary_Program/";

    system("mkdir -p ~/Vocabulary_Program/");

#else
    OS_Name = "Android";

#endif

    head = NULL;
    FileChanged = false;
    VocabListLocation = FolderLocation + SettingsCheck(Exam_lengths);
    chapter_counter = 0;
    Word_Count = 0;
    FoundCard = false;

    CheckFile();
}

string Vocab_Card::SettingsCheck(Dynamic_Array<int>& Exam_lengths)
{
    string tempLocation = FolderLocation;
    string fileName, textFile;
    //char answer;
    bool IsEmpty = false;
    int ExamCount = 0;

    tempLocation = tempLocation + "settings.txt";

    ifstream settings;
    settings.open(tempLocation);

    if (settings)
    {
        settings >> fileName;
        settings.close();
    }

    else
    {
        settings.close();



        IsEmpty = true;
        cout << "Hello, Welcome to the installer program" << endl
            << "What is the name of class that you are creating flashcards for? (Example Geology 201, Physics 225, etc.)" << endl;

        getline(cin,fileName);

        fileName = fileName + ".txt";
        SpaceReplacer(fileName);
        ofstream saveLocation(tempLocation);
        saveLocation << fileName;
        saveLocation.close();

        FileChanged = true;
        SetExamInfo();

    }

    if (IsEmpty)
    {
        ExamCount = Exam_lengths.size();
        textFile = FolderLocation + fileName;
        ofstream Flashcard(textFile);
        Flashcard << "ExamCount=" << ExamCount << endl;


        for (int i = 0; i < ExamCount; i++)
        {
            if (i == 0)
                Flashcard << "Exam_1: 1-" << Exam_lengths[i] << endl;

            else
                Flashcard << "Exam_" << i + 1 << ": " << Exam_lengths[i - 1] + 1 << "-" << Exam_lengths[i] << endl;

        }
        Flashcard << endl << "Flash_Card_Size=0";
        Flashcard.close();

        Exam_lengths.Empty_Array();
    }

    return fileName;
}

void Vocab_Card::ShowExamInfo()
{
    for (int i = 0; i < Exam_lengths.size(); i++)
       {
        cout << "Exam " << i+1 << " : chapters ";
           if (i == 0)
                cout << "1 ";
           else
                cout << Exam_lengths[i-1]+1;
           cout << " - " << Exam_lengths[i] << endl;

        }
}

void Vocab_Card::SetExamInfo()
{
    char answer;
    FileChanged = true;
    int ExamCount, temp;
    Exam_lengths.Empty_Array();
    int tempValue;

    cout << "How many exams do you have? (including the final exam)" << endl;
    cin >> ExamCount;

    do {
        cout << endl << "is the Final Exam cumulative? (Y/N) ";
        cin >> answer;

    } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');


    if (answer == 'y' || answer == 'Y')
        ExamCount--;


    for (int i = 0; i < ExamCount; i++)
    {
        if (i == 0)
          {
            cout << endl << "Exam 1 starts at chapter 1 and ends at what chapter? ";
            tempValue = 1;
          }
        else
          {
            cout << endl << "Exam " << i + 1 << " starts at chapter " << Exam_lengths[i - 1] + 1 << " and ends at what chapter? ";
            tempValue = Exam_lengths[i-1]+1;
            }

        do{
        cin >> temp;
        cin.ignore();
        }while(temp <= tempValue);

        Exam_lengths.push_back(temp);

    }
}

void Vocab_Card::DeleteTextFile(int fileOption)
{
    int value;
    string fileName = TextOptions[fileOption-1];
    fileName = FolderLocation + fileName;
    string settings = FolderLocation + "settings.txt";
    FileChanged = true;

    remove(fileName.c_str());

    if(fileName == VocabListLocation)
    {
        ShowTextFiles();
      do{
        cout << "What do you want your default file to be?" << endl;
        cin >> value;
      }while(value < 0 || GetTextFileSize() < value);


        ofstream change(settings);
        change << TextOptions[value - 1];
        change.close();
    }





}

void Vocab_Card::SetDefaultFile(string name)
{
   name = name + ".txt";
   SpaceReplacer(name);
   string textFile = FolderLocation + "output.txt";
   string settings = FolderLocation + "settings.txt";
   string input;
   FileChanged = true;


   if (OS_Name == "Windows")
   {
       system("dir /b \"C:\\Users\\%USERNAME%\\Documents\\Vocabulary_Program\" > \"C:\\Users\\%USERNAME%\\Documents\\Vocabulary_Program\\output.txt\"");

   }

   else if (OS_Name == "Linux")
   {
       system("ls ~/Vocabulary_Program/ >> ~/Vocabulary_Program/output.txt");
   }


    if (TextOptions.size() > 0)
        TextOptions.Empty_Array();

    ifstream Text(textFile);
    while(!Text.eof())
    {
        Text >> input;
        if (input == "output.txt" || input == "settings.txt")
            {}
        else
            TextOptions.push_back(input);


    }

    Text.close();

    remove(textFile.c_str());

    for (int i = 0; i < TextOptions.size(); i++)
    {
        if (name == TextOptions[i])
        {
            ofstream change(settings);
            change << TextOptions[i];
            change.close();

        }
    }

}

string Vocab_Card::GetDefaultFile()
{
    string Default;
    string fileLocation = FolderLocation + "settings.txt";

    ifstream FileName(fileLocation);
    FileName >> Default;
    FileName.close();

    return Default;


}


void Vocab_Card::SetExamToFile(string textName)
{
    SpaceReplacer(textName);
    int ExamCount, temp;
    char answer;
    Dynamic_Array<int> Exam_info;
    FileChanged = true;

    cout << "How many exams do you have?" << endl;
    cin >> ExamCount;

    do {
        cout << endl << "is the Final Exam cumulative? (Y/N) ";
        cin >> answer;

    } while (answer != 'Y' && answer != 'y' && answer != 'N' && answer != 'n');


    if (answer == 'y' || answer == 'Y')
        ExamCount--;
    else
    {
    }


    for (int i = 0; i < ExamCount; i++)
    {
        if (i == 0)
            cout << endl << "Exam 1 starts at chapter 1 and ends at what chapter? ";

        else
            cout << endl << "Exam " << i + 1 << " starts at chapter " << Exam_info[i - 1] + 1 << " and ends at what chapter? ";

        cin >> temp;
        Exam_info.push_back(temp);
    }


    string text = textName + ".txt";
    string TextFile = FolderLocation + text;
    ofstream Setup(TextFile);

    Setup << "ExamCount=" << ExamCount << endl;


    for (int i = 0; i < ExamCount; i++)
    {
        if (i == 0)
            Setup << "Exam_1: 1-" << Exam_info[i] << endl;

        else
            Setup << "Exam_" << i + 1 << ": " << Exam_info[i - 1] + 1 << "-" << Exam_info[i] << endl;

    }
    Setup << endl << "Flash_Card_Size=0";
    Setup.close();

    Exam_info.Empty_Array();

}


void Vocab_Card::CheckFile()
{

    string input;
    string Vocab_word;
    int Exam_Count;
    int def_count;

    ifstream readFile;
    int index = 0, value = 0;

    readFile.open(VocabListLocation);

    if (readFile)
    {
        readFile.ignore(20, '=');

        readFile >> Exam_Count;

        for (int i = 0, temp; i < Exam_Count; i++)
        {
            readFile.ignore(20, '-');
            readFile >> temp;
            Exam_lengths.push_back(temp);
        }

        readFile.ignore(20, '=');

        Node* current = head;

        readFile >> Word_Count;

        if (Word_Count == 0)
        {
            readFile.close();
            return;
        }
        //cout << "Word Count is " << Word_Count;

        while (!readFile.eof())
        {
            readFile.ignore(20, '=');
            readFile >> value;
            chapter_sizes.push_back(value);
            chapter_counter++;


            for (int i = 0, chpt_size = chapter_sizes[index]; i < chpt_size; i++) {



                readFile.ignore(2, '-');
                getline(readFile, Vocab_word);
                //cout << "Word is " << temp->Vocab_Word << endl;

                readFile.ignore(20, '=');
                readFile >> def_count;

                for (int q = 0; q < def_count; q++)
                {
                    readFile.ignore(3, '-');
                    getline(readFile, input);
                    definitions.push_back(input);
                }

                Node* newNode = new Node(Vocab_word, definitions, def_count);

                if (current == NULL)
                {
                    head = current = newNode;
                    newNode = NULL;
                    delete newNode;
                }
                else
                {
                    current->next = newNode;
                    newNode = NULL;
                    delete newNode;
                    current = current->next;

                }

                if (i == 0)
                {
                    chapterPointer.push_back(current);
                }

                definitions.Empty_Array();

            }
            index++;
        }
        current = NULL;
        delete current;

    }

    else
    {
        string temp = FolderLocation + "settings.txt";

        ifstream fileName(temp);
        fileName >> temp;
        fileName.close();

        cout << "ERROR!! File not found!!" << endl
            << "If you have moved the file, " << temp << endl
            << "return it back into the directory " << endl << endl << FolderLocation
            << endl << endl << "and try again" << endl << endl;

        cout << "If the file has been deleted, simply delete " << endl
            << "settings.txt from the same folder to fix the issue" << endl << endl;


        cout << "Press the enter key to exit" << endl;

        cin.get();
        system("pause");
        exit(0);

    }

    readFile.close();


}

void Vocab_Card::First_Install()
{



}

int Vocab_Card::GetTextFileSize()
{
    return TextOptions.size();
}

void Vocab_Card::ShowTextFiles()
{
    string textFile = FolderLocation + "output.txt";
    string input;

    if (OS_Name == "Windows")
    {
        system("dir /b \"C:\\Users\\%USERNAME%\\Documents\\Vocabulary_Program\" > \"C:\\Users\\%USERNAME%\\Documents\\Vocabulary_Program\\output.txt\"");

    }

    else if (OS_Name == "Linux")
    {
        system("ls ~/Vocabulary_Program/ >> ~/Vocabulary_Program/output.txt");
    }


    if (TextOptions.size() > 0)
            TextOptions.Empty_Array();

        ifstream Text(textFile);
        while(!Text.eof())
        {
            Text >> input;
            if (input == "output.txt" || input == "settings.txt")
                {}
            else
                TextOptions.push_back(input);


        }

        Text.close();

        remove(textFile.c_str());


    for (int i = 0; i < TextOptions.size(); i++)
    {
        cout << i+1 << ". " << TextOptions[i] << endl;
    }
}

bool Vocab_Card::ChangeDefaultTextFile(int value)
{
    string Settings = FolderLocation + "settings.txt";
    if(TextOptions.size() == 0)
        return false;

    value--;

    ofstream SettingsChange(Settings);
    SettingsChange << TextOptions[value];
    SettingsChange.close();

    TextOptions.Empty_Array();

    return true;
}



int Vocab_Card::GetChapterCount()
{
    return chapter_counter;

}

bool Vocab_Card::HasFileChanged()
{
    return FileChanged;
}

int Vocab_Card::GetExamCount()
{
    return Exam_lengths.size();
}

bool Vocab_Card::CardFound()
{
    return FoundCard;
}

int Vocab_Card::GetSearchSize()
{
    return SearchValues.size();
}

void Vocab_Card::ClearScreen()
{
    if (OS_Name == "Windows")
        system("CLS");  //batch command
    else
        system("clear"); //bash command
}

void Vocab_Card::PauseProgram()
{
    if (OS_Name == "Windows")
        system("pause");

    else if (OS_Name == "Linux")
    {
        cout << endl << "Please press the Enter key to continue" << endl;
        cin.get();
    }
}

void Vocab_Card::AddCard(int chapter, string vocab_word, Dynamic_Array<string> &def)
{
    bool NewChapter = 0;
    int add_to_index = 0;
    string answer = "0";
    ClearScreen();
    int New_Chapter = GetChapterCount()+ 1;

    Node* newNode = new Node(vocab_word, def, def.size());

    FileChanged = true;

    if (chapter != New_Chapter)
        chapter--;

    if (chapter == New_Chapter)
    {
        chapter_sizes.push_back(0);
        chapter_counter++;
        chapter = chapter_counter - 1;
        NewChapter = true;
    }

    ClearScreen();
    add_to_index = 0;
    chapter_sizes[chapter]++;
    Word_Count++;

    for (int q = 0; q <= chapter; q++)
    {
        add_to_index += chapter_sizes[q];
    }

    //cin.ignore();
    answer = "0";

    if (NewChapter)
    {
        chapterPointer.push_back(newNode);
        NewChapter = false;
    }

    if (head == NULL)
    {

        head = newNode;
        newNode = NULL;
    }

    else
    {
        Node* current = head;

        for (int i = 1; i != (add_to_index - 1) && current->next != NULL;i++)
        {
            //cout << "I'm at vocab word " << current->Vocab_Word << endl;
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        newNode = NULL;
        current = newNode;

    }


    delete newNode;


}

void Vocab_Card::ShowChapters()
{
    for (int p = 0; p < chapter_counter; p++)
    {
        cout << p + 1 << ". Chapter " << p + 1 << endl;

    }

}


void Vocab_Card::ShowSearch()
{
    for (int i = 0; i < GetSearchSize(); i++)
      {

        cout << i+1 << ". " << SearchValues[i]->Vocab_Word << endl;
      }
}


void Vocab_Card::ShowDefinitions(string Name, int& definitionCount)
{
    int index = 0;

    for (int i = 0; i < GetSearchSize(); i++)
    {
       if (SearchValues[i]->Vocab_Word == Name)
          {
           index = i;
           break;
          }
    }
    definitionCount = SearchValues[index]->definition_count;
    for (int i = 0; i < definitionCount; i++)
    {
        cout << i+1 << ". " << SearchValues[index]->definitions[i] << endl;
    }

    cout << endl << endl;
}

void Vocab_Card::ShowFlashCard(string answer)
{
    int index = 0;

    for (int i = 0; i < GetSearchSize(); i++)
    {
       if (SearchValues[i]->Vocab_Word == answer)
          {
           index = i;
           break;
          }
    }
    cout << "Vocab_Word: " << SearchValues[index]->Vocab_Word << endl;

    for (int i = 0; i < SearchValues[index]->definition_count; i++)
    {
        cout << "-" << SearchValues[index]->definitions[i] << endl;
    }

    cout << endl << endl;

}

void Vocab_Card::SetVocabWord(string Original, string NewVocabWord)
{
    FileChanged = true;
    int index = 0;

    for (int i = 0; i < GetSearchSize(); i++)
    {
       if (SearchValues[i]->Vocab_Word == Original)
          {
           index = i;
           break;
          }
    }

    SearchValues[index]->Vocab_Word = NewVocabWord;
}


void Vocab_Card::SetDefinition(string VocabName, int definitionNumber, string value)
{
    FileChanged = true;
    int index = 0;

    for (int i = 0; i < GetSearchSize(); i++)
    {
       if (SearchValues[i]->Vocab_Word == VocabName)
          {
           index = i;
           break;
          }
    }

    SearchValues[index]->definitions[definitionNumber-1] = value;


}

string Vocab_Card::GetSearchValue(int index)
{
    return SearchValues[index]->Vocab_Word;
}

void Vocab_Card::MultipleChoiceGame(int chapter)
{
    int All_Chapters = GetChapterCount() + GetExamCount() + 1;
    int Normal_Chapters = GetChapterCount();
    int index = 0;
    int exam_number=0;
    Node* temp, *temp2;
    int MinChapter, MaxChapter;


     if (head == NULL)
      {
         cout << "No Flashcards have been added" << endl;
         PauseProgram();
         return;
      }

     if (chapter == All_Chapters)
     {
         index = Word_Count;
         temp = head;
         MinChapter = 0;
         MaxChapter = GetChapterCount()-1;
     }

     else if (chapter > Normal_Chapters)
     {

          exam_number = chapter - Normal_Chapters;

         if (exam_number == 1)
             MinChapter = 0;

         else
         {
             MinChapter = Exam_lengths[exam_number -2];

             if (MinChapter > GetChapterCount())
             {
                 cout << "There are no notecards for this exam" << endl;
                 PauseProgram();
                 return;
             }
         }

         MaxChapter = Exam_lengths[exam_number-1]-1;

         temp = chapterPointer[MinChapter];

         for(int i = MinChapter; i <= MaxChapter && i < GetChapterCount(); i++)
         {
             index += chapter_sizes[i];
         }

     }

     else
     {
         temp = chapterPointer[chapter -1];
         index = chapter_sizes[chapter -1];
         MinChapter = chapter-1;
         MaxChapter = chapter;

     }

     char answer;
     int Index_Move_Count;
     bool FirstLoop = true;
     bool* valueUsed = new bool[index];
     int unused_indexes = index;
     int total_indexes = 0;
     int q = 0;
     Dynamic_Array<int> Vocab_indexes;
     Dynamic_Array<string> Definition_Choices;
     int correct_Choice;
     int option_Count;
     int disposable;
     string correct_definition;
     char letter_option;
     int vocab_index;
     bool isUnique = true;
     int total_score = 0;
     int total_correct = 0;
     double total_percent = 0;
     int point_difficulty = 2;

 do{
    vocab_index = 0;
    Vocab_indexes.Empty_Array();
    Definition_Choices.Empty_Array();

     if(unused_indexes == 0 || FirstLoop == true)
     {
         FirstLoop = false;
         unused_indexes = index;

         for (int i = 0; i < index; i++)
            valueUsed[i] = false;
     }

         ClearScreen();
         //cout << "Hit Enter order see the definitions, or type \"t\"  and Enter to exit" << endl;

     temp2 = temp;

    Index_Move_Count = rand() % unused_indexes;

     for(int i = 0; i <= Index_Move_Count;)
     {
         if (i == Index_Move_Count && valueUsed[total_indexes] == false )
         {
             valueUsed[total_indexes] = true;
             unused_indexes--;
             break;
         }
         else if (valueUsed[total_indexes] == false)
         {
                 i++;
         }
         total_indexes++;
     }


     Index_Move_Count = total_indexes;
     vocab_index = Index_Move_Count;
     Vocab_indexes.push_back(total_indexes);
     total_indexes = 0;

     for (int min = MinChapter;min < MaxChapter && Index_Move_Count > chapter_sizes[min]; min++)
     {
       Index_Move_Count -= chapter_sizes[min];
       temp2=chapterPointer[min + 1];
     }

     for (int i = 0; i < Index_Move_Count; i++)
     {
         temp2 = temp2->next;
     }

     cout << endl << "Vocab Word: " << temp2->Vocab_Word << endl;



     if (index >= 4)
     {
         option_Count = 4;
     }
     else if (index == 0)
     {
        cout << "There is no flashcards available" << endl;
        PauseProgram();
        return;

     }

     else
     {
         option_Count = index;
     }


    correct_Choice = random() % option_Count;


    disposable = rand() % temp2->definition_count;

    correct_definition = temp2->definitions[disposable];

    Definition_Choices.push_back(temp2->definitions[disposable]);

  for (int i = 0; i < (option_Count-1); i++)
   {

      temp2= temp;

      do{
          q = Vocab_indexes.size()-1;
          isUnique = true;
      Index_Move_Count = rand() % index;
        while(q >= 0)
        {
            if (Index_Move_Count == Vocab_indexes[q])
            {
                isUnique = false;
                break;
            }
            q--;
        }

        }while(isUnique == false);

      Vocab_indexes.push_back(Index_Move_Count);


        total_indexes = 0;

        for (int min = MinChapter;min < MaxChapter && Index_Move_Count > chapter_sizes[min]; min++)
         {
            Index_Move_Count -= chapter_sizes[min];
            temp2=chapterPointer[min + 1];
         }

        for (int i = 0; i < Index_Move_Count; i++)
         {
          temp2 = temp2->next;
         }

        disposable = rand() % temp2->definition_count;
        Definition_Choices.push_back(temp2->definitions[disposable]);
    }

    Definition_Choices[0] = Definition_Choices[correct_Choice];
    Definition_Choices[correct_Choice] = correct_definition;

    letter_option = 'A';
    cout << endl;
    for (int i = 0; i < option_Count; i++)
    {
       cout << letter_option << ". " << Definition_Choices[i] << endl << endl;
       letter_option++;
    }

    do{
    cin >> answer;
    cin.ignore();

   }while(answer < 'A' && answer > 'A' + (option_Count-1));

   if (answer == 'A' + (correct_Choice) || answer == ('A'+ correct_Choice)+ 32)
   {
        cout << "CORRECT!!" << endl;
    total_correct += point_difficulty;
   }

   else
   {
        cout << "INCORRECT!!, The answer was letter " << char('A'+ (correct_Choice)) << endl;
   }

   total_score += point_difficulty;

    cout << "press \"Enter\" to play again, or \"t\" and \"Enter\" to exit." << endl;
    cout << "Your input: ";
    answer = getchar();
   }while( answer != 't' && answer != 'T');



 total_percent = (double(total_correct)/double(total_score)) * 100.00;
 int temp_percent = int(total_percent);

 ClearScreen();
  cout << "You got " << total_correct << " out of " << total_score << " points." << endl
       << "resulting in a " ;
  cout << fixed << setprecision(1) << total_percent << "% " << "(" << TestGrade(temp_percent) << ")" << endl;

 cin.ignore();
 PauseProgram();

 temp = temp2 = NULL;
 delete[] valueUsed;
 delete temp;
 delete temp2;


}

string Vocab_Card::TestGrade(int total_percent)
{
  char tempGrade = 'A';
  string letterGrade = "F";

  for (double CurrentGrade = 90; CurrentGrade > 50; CurrentGrade-= 10, tempGrade = tempGrade + 1)
    {
      if (total_percent >= (CurrentGrade+ 5))
    {
      letterGrade = tempGrade;
      letterGrade = letterGrade + "+";
      break;
    }
      else if (total_percent > (CurrentGrade+ 3))
    {
      letterGrade = tempGrade;
      break;
    }
      else if (total_percent >= CurrentGrade)
    {
      letterGrade = tempGrade;
      letterGrade = letterGrade + "-";
      break;

    }

  }
  return letterGrade;
}

void Vocab_Card::TemporaryTest(int chapter)
{
   int All_Chapters = GetChapterCount() + GetExamCount() + 1;
   int Normal_Chapters = GetChapterCount();
   int index = 0;
   int exam_number=0;
   Node* temp, *temp2;
   int MinChapter, MaxChapter;


    if (head == NULL)
     {
        cout << "No Flashcards have been added" << endl;
        PauseProgram();
        return;
     }

    if (chapter == All_Chapters)
    {
        index = Word_Count;
        temp = head;
        MinChapter = 0;
        MaxChapter = GetChapterCount()-1;
    }

    else if (chapter > Normal_Chapters)
    {

         exam_number = chapter - Normal_Chapters;

        if (exam_number == 1)
            MinChapter = 0;

        else
        {
            MinChapter = Exam_lengths[exam_number -2];

            if (MinChapter > GetChapterCount())
            {
                cout << "There are no notecards for this exam" << endl;
                PauseProgram();
                return;
            }
        }

        MaxChapter = Exam_lengths[exam_number-1]-1;

        temp = chapterPointer[MinChapter];

        for(int i = MinChapter; i <= MaxChapter && i < GetChapterCount(); i++)
        {
            index += chapter_sizes[i];
        }

    }

    else
    {
        temp = chapterPointer[chapter -1];
        index = chapter_sizes[chapter -1];
        MinChapter = chapter-1;
        MaxChapter = chapter;

    }



    char answer;
    int Index_Move_Count;
    bool FirstLoop = true;
    bool* valueUsed = new bool[index];
    int unused_indexes = index;
    int total_indexes = 0;

do{

    if(unused_indexes == 0 || FirstLoop == true)
    {
        FirstLoop = false;
        unused_indexes = index;

        for (int i = 0; i < index; i++)
           valueUsed[i] = false;
    }

        ClearScreen();
        cout << "Hit \"Enter\" order see the definitions. After the definitions have been shown," << endl
             << "you can type \"t\" and \"Enter\" to exit" << endl;

    temp2 = temp;

   Index_Move_Count = rand() % unused_indexes;


    for(int i = 0; i <= Index_Move_Count;)
    {
        if (i == Index_Move_Count && valueUsed[total_indexes] == false )
        {
            valueUsed[total_indexes] = true;
            unused_indexes--;
            break;
        }
        else if (valueUsed[total_indexes] == false)
        {
                i++;
        }
        total_indexes++;
    }


    Index_Move_Count = total_indexes;
    total_indexes = 0;

    for (int min = MinChapter;min < MaxChapter && Index_Move_Count > chapter_sizes[min]; min++)
    {
      Index_Move_Count -= chapter_sizes[min];
      temp2=chapterPointer[min + 1];
    }

    for (int i = 0; i < Index_Move_Count; i++)
    {
        temp2 = temp2->next;
    }

    cout << endl << "Vocab Word: " << temp2->Vocab_Word << endl;
    cin.get();

    for (int i = 0; i < temp2->definition_count; i++)
    {
        cout << "-" << temp2->definitions[i] << endl;
    }




   answer = cin.get();

  }while(answer != 't' && answer != 'T');

    temp = temp2 = NULL;

    delete[] valueUsed;
    delete temp;
    delete temp2;
}


void Vocab_Card::Exit()
{

    if (HasFileChanged() == false)
        return;

    ofstream output;
    int ExamCount = Exam_lengths.size();
    int index = 0;
    int total = 0;
    output.open(VocabListLocation);

    output << "ExamCount=" << ExamCount << endl;


    for (int i = 0; i < ExamCount; i++)
    {
        if (i == 0)
            output << "Exam_1: 1-" << Exam_lengths[i] << endl;

        else
            output << "Exam_" << i + 1 << ": " << Exam_lengths[i - 1] + 1 << "-" << Exam_lengths[i] << endl;

    }

    Exam_lengths.Empty_Array();




    output << endl << "Flash_Card_Size=" << Word_Count << endl;
    for (int j = 0;j < chapter_counter; j++)
    {
        output << "Chapter " << j + 1 << " Size=" << chapter_sizes[j] << endl;

        for (int q = 0, chpt_size = chapter_sizes[index];q < chpt_size;q++)
        {
            Node* temp = head;
            output << "-" << temp->Vocab_Word << endl;
            output << "\tDescription-Size=" << temp->definition_count << endl;

            for (int i = 0; i < temp->definition_count; i++)
            {
                output << "\t-" << temp->definitions[i];
                if (total != (Word_Count - 1))
                    output << endl;
                else if (total == (Word_Count - 1) && i != (temp->definition_count - 1))
                    output << endl;
                else
                {
                }
            }
            if (total != (Word_Count - 1))
                output << endl;

            for (int j = 0; j < temp->definition_count; j++)
            {
                temp->definitions.pop_back();
            }

            head = head->next;
            delete temp;
            total++;

        }
        index++;
    }
    output.close();

    for (int q = 0; q < chapter_counter; q++)
    {
        chapter_sizes.pop_back();
        chapterPointer.pop_back();
    }

}

void Vocab_Card::Help(char value)
{
    ClearScreen();
    switch(value)
    {
        case 'A':
            cout << "The \"Add a Vocab Card\" section allows you as the user to add a new vocab card to" << endl
                 << "your current class that you are in.You may add the vocab card to any chapter you"<< endl
                 << "have already created, or can add it into a new chapter. You will first add the" << endl
                 << "vocab word, and then you will be able to add as many definitions as you want" << endl
                 << "into the card by just pressing enter. If you wanted to add another card to that" << endl
                 << "same chapter, in the definitions, type \"t\" and enter, and you will be allowed" << endl
                 << "to add another card to the same chapter. Typing \"e\" and enter will return" << endl
                 << "back to the original menu" << endl;
            break;
        case 'B':
            cout << "The \"Test your Knowledge\" section tries to test how well you really know" << endl
                 << "about your class vocabulary words and definitions. It will first ask you what" << endl
                 << "chapter you want to study for, or you could study for all the chapters that" << endl
                 << "are covered in one of your Exams. Once chosen, you will be prompted to play" << endl
                 << "the regular randomizer test or the more exciting Multiple choice test. The" << endl
                 << "regular randomizer game will show you your vocab word and you will have to" << endl
                 << "guess the definitions for it. The multiple choice game will test you like a" << endl
                 << "multiple choice test. You will have choices to chose from for the vocab" << endl
                 << "definition, and once you exit the game, it will give you a report card of" << endl
                 << "how well you performed on the test with the score you got on it with the" << endl
                 << "overall percentage." << endl;
            break;
        case 'C':
            cout << "The \"Edit a Flashcard\" section allows you to find any flashcard you" << endl
                 << "have in your current class and edit it however you would like to. You" << endl
                 << "could change the vocab word or any of the individual definitions to" << endl
                 << "whatever you would like. Finally, you could delete the flashcard all" << endl
                 << "together if you don't want it anymore." << endl;
            break;
        case 'D':
            cout << "The \"Display flash cards\" section shows all the flashcards for your" << endl
                 << "class based on which chapter you would like to view. You can view a" << endl
                 << "single chapters worth of flashcards, an entire Exams worth of" << endl
                 << "flashcards or see all of the flashcards available." << endl;
            break;
        case 'E':
            cout << "The \"Configure your Default Class\" section allows you to change your" << endl
                 << "default file to another class. You also have the options to delete a" << endl
                 << "class or add a new class in this section. In order for any of these" << endl
                 << "options to go into effect, you will have to restart the program." << endl;
            break;
        case 'F':
            cout << "The \"Change Exam Information\" section allows you change what chapters" << endl
                 << "cover each exam for your current class. It will first show you your" << endl
                 << "current exam setup and ask you wanted to change that setup or not. If" << endl
                 << "you choose \"No\" it will return you back to the main menu, otherwise" << endl
                 << "it will allow you to make the changes" << endl;
            break;
        case 'G':
            cout << "Your files are currently located in the current directory:" << endl << endl
                 << FolderLocation << endl << endl
                 << "It is highly advised NOT to remove/edit any of the files in the" << endl
                 << "directory, but if you plan on using this program on multiple" << endl
                 << "computers, and want to use the same file between the two locations," << endl
                 << "make sure to copy the class file from this directory, and then paste" << endl
                 << "it into the other computers directory. From there, you can change" << endl
                 << "your default class file in the programs menu." << endl << endl;

            cout << "The class text file has all of the vocab words/definitions neatly" << endl
                 << "organized in its proper chapters. Please DO NOT edit this file!!" << endl
                 << "Changing certain spots of the file could cause serious issues when" << endl
                 << "trying to restart the program. You have been warned!!" << endl;
            break;
    }


}

void Vocab_Card::AboutMe()
{
    ClearScreen();

    cout << "Hello, my name is Roy Redman IV. I am a programmer at CSUF. I worked on this" << endl
         << "program to benifit myself and others who are currently trying to study for their" << endl
         << "next big exam and don't want to take the time and hassle of writting their" << endl
         << "flashcards by hand and then end up loosing it later when its needed the most." << endl
         << "This program works natively with any Windows or linux Operating System." << endl
         << "I should be able to transfer this over to Macintosh in the future. Please" << endl
         << "let me know of your thoughts about my program by emailing me at any time. Enjoy!" << endl << endl
         << "Email: royredman4@gmail.com" << endl;
}

void Vocab_Card::Display(int chapter)
{
    Node* temp;
    //ClearScreen();
    int index, maxIndex;
    int All_Chapters = GetChapterCount() + GetExamCount() + 1;

    if (chapter > chapter_counter && chapter != All_Chapters)
    {
        chapter = chapter - chapter_counter;
        DisplayExams(chapter);
        return;

    }


    if (chapter == All_Chapters)
    {
        index = 0;
        maxIndex = chapter_counter;
        temp = head;
    }
    else
    {
        maxIndex = chapter;
        index = chapter - 1;
        temp = chapterPointer[index];

    }

    for (int j = index;j < maxIndex; j++)
    {
        cout << endl << "Chapter " << j + 1 << " :" << endl;

        for (int q = 0, chpt_size = chapter_sizes[index];q < chpt_size;q++)
        {
            cout << endl << "Vocab word is: " << temp->Vocab_Word << endl;

            for (int i = 0; i < temp->definition_count; i++)
            {
                cout << "-" << temp->definitions[i] << endl;
            }
            temp = temp->next;
        }
        index++;
    }

    temp = NULL;
    delete temp;

}

void Vocab_Card::DisplayExams(int ExamNumber)
{

    int min, max;
    if (ExamNumber == 1)
        min = 1;
    else
        min = Exam_lengths[ExamNumber - 2] + 1;

    max = Exam_lengths[ExamNumber - 1];

    while (min <= max && min <= chapter_counter)      //Check to see if this keeps from going too far over how many chapters they created
    {
        Display(min);
        min++;
    }
}

string Vocab_Card::EditCard(string answer)
{
    if(GetSearchSize() > 0)
     {
        SearchValues.Empty_Array();
        FoundCard = false;
     }
    ClearScreen();
    string value;
    int chapter = 0;

    Node* temp = SearchForCard(answer, chapter);

    if (temp != NULL)
    {
        value = temp->Vocab_Word;
        return value;
    }

    else
        return "NULL";

}


Node* Vocab_Card::SearchForCard(string name, int& index)
{
    //ClearScreen();
    Node* temp2 = head;
    int StrSize = name.length();
    int errorLimit = double(.2) * double(StrSize);
    double errorCounter = 0;
    char name1[40 + 1];
    char name2[50];
    //cout << "Looking for " << name << endl;
    index = 1;

    while (temp2 != NULL)
    {
        if (temp2->Vocab_Word == name)
        {
            //cout << "Found IT";
            FoundCard = true;
            SearchValues.Empty_Array();
            SearchValues.push_back(temp2);
            return temp2;
        }

       // else if (temp2 == chapterPointer[index])
            index++;

        errorCounter = 0;
        for (int i = 0; i < StrSize && errorLimit >= errorCounter; i++)
        {
            name1[i] = tolower(name[i]);
            name2[i] = tolower(temp2->Vocab_Word[i]);
            if (name1[i] != name2[i])
            {
                errorCounter++;
            }

        }
        //cout << "Im in " << temp2->Vocab_Word <<endl;
        if (errorCounter == 0)
        {
            SearchValues.Empty_Array();
            SearchValues.push_back(temp2);
            FoundCard = true;
            break;
        }
        else if (errorLimit >= errorCounter)
            SearchValues.push_back(temp2);

        temp2 = temp2->next;
    }

    return temp2;

}


void Vocab_Card::Delete(string value)
{
    FileChanged = true;
    Node* temp = head;

    int index = 1;
    while(temp->next->Vocab_Word != value && temp != NULL)
    {
        if (temp == chapterPointer[index])
            index++;
        temp = temp->next;
    }

    if (temp == NULL)
    {}
    else
      {
        Node* temp2 = temp->next;
        temp->next = temp2->next;

        delete temp2;
        temp = NULL;

        chapter_sizes[index-1]--;
      }

    delete temp;
    Word_Count--;
    SearchValues.Empty_Array();
}


void Vocab_Card::SpaceReplacer(string& fileName)
{
    int i = fileName.size();

    for (int j = 0; j < i; j++)
    {
        if (fileName[j] == ' ')
            fileName[j] = '_';
    }

}

Vocab_Card::~Vocab_Card()
{
    delete head;
}
