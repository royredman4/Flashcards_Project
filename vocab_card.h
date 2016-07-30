#ifndef VOCAB_CARD_H
#define VOCAB_CARD_H

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

#elif WIN32 || _WIN32
#include <windows.h>
#include <Lmcons.h>
#include <shlobj.h>

#endif

#include <fstream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <iomanip>
#include <cstddef>
#include "dynamic_array.h"


struct Node {

    string Vocab_Word;
    Dynamic_Array<string> definitions;
    //vector<string> definitions;
    int definition_count;
    Node* next;
    Node(string Vocab, Dynamic_Array<string> &define, int def_Count) : Vocab_Word(Vocab), definition_count(def_Count), next(0)
    {
        for (int i = 0; i < definition_count; i++)
        {
            definitions.push_back(define[i]);
        }

    }

};



class Vocab_Card
{


public:

    Vocab_Card();
    string EditCard(string);
    void TemporaryTest(int);
    void AddCard(int, string, Dynamic_Array<string>&);
    void MultipleChoiceGame(int);
    void ClearScreen();
    void PauseProgram();
    void Display(int);
    void ShowChapters();
    void ShowSearch();
    void ShowFlashCard(string);
    void ShowExamInfo();
    void SetExamToFile(string);
    void ShowDefinitions(string, int&);
    void SetDefinition(string, int, string);
    void SetVocabWord(string, string);
    void SetDefaultFile(string);
    string GetDefaultFile();
    string GetSearchValue(int);
    int GetChapterCount();
    int GetExamCount();
    int GetSearchSize();
    void TestYourKnowledge(int);
    void Exit();
    void SetExamInfo();
    void DeleteTextFile(int);
    bool CardFound();
    void ShowTextFiles();
    bool ChangeDefaultTextFile(int);
    int GetTextFileSize();
    void Delete(string Vocab_Word);
    void Help(char);
    void AboutMe();

    ~Vocab_Card();

private:
    bool FileChanged;
    Node* head;
    string OS_Name;
    string VocabListLocation;
    string FolderLocation;
    Dynamic_Array<int> Exam_lengths;
    Dynamic_Array<string> TextOptions;
    bool FoundCard;
    int Word_Count;
    int chapter_counter;

    Dynamic_Array<int> chapter_sizes;
    Dynamic_Array<Node*> chapterPointer;
    Dynamic_Array<Node *> SearchValues;
    Dynamic_Array<string> definitions;

    bool HasFileChanged(); //Checks if the list has changed so we don't have to overwrite the file if not changed
    void DeleteCard(int, Node*);
    void SpaceReplacer(string&);
    string TestGrade(int);
    string GetSettingsFile();
    void CheckFile();
    string SettingsCheck(Dynamic_Array<int>&);
    void DisplayExams(int);
    Node* SearchForCard(string, int&);

    void Delete(int chapter, string Vocab_Word);  //First will delete if there is a chapter identified, otherwise, it will search for it regularly

};

#endif // VOCAB_CARD_H
