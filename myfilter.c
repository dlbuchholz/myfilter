///////////////////////////////////////////////////////////
// myfilter.c                                            //
//                                                       //
// Dieser Quellcode beinhaltet die Implementation        //
// eines Filters in der Programmiersprache C, welcher es //
// ermöglicht Wörter, Zeichen und Zeilen einer           //
// Datei zu zählen.                                      //
//                                                       //
// Erstellt am: 10. - 13.10.2022                         //
//       Autor: Dennis Lucas Buchholz                    //
///////////////////////////////////////////////////////////

#include <stdio.h>
#include <unistd.h>

// Struktur in der die Flags gespeichert werden
struct mode {
    int characters;
    int words;
    int lines;  
};

void display_usage() {
    printf("Usage: myfilter [options] [-i] file\n");
    printf("Options:\n");
    printf(" -c     Count characters of file\n");
    printf(" -w     Count words of file\n");
    printf(" -l     Count lines of file\n");
}

int main (int argc, char **argv) {
    // Initialisiere die Zählervariablen für Zeichen, Wörter und Zeilen
    int characters = 0, words = 1, lines = 0;
    char* file_name = ""; // Variable für den Dateinamen
    FILE* file = stdin; // Dateipointer (standardmäßig zeigt dieser auf die Standardeingabe)

    // Initialisiere die Flags
    struct mode flags;
    flags.characters = 0;
    flags.words = 0;
    flags.lines = 0;

    // Lese alle an das Programm übergebene Parameter mit Hilfe von getopt
    // und setze die dazugehörigen Flags; die While-Schleife ist erst beendet
    // sobald durch '-1' signalisiert wurde, dass keine weiteren Parameter vorliegen
    int option;
    while((option = getopt(argc, argv, "cwli")) != -1 ) {
        switch (option) {
            case 'c':
            // Wenn ein -c vorliegt, setze 'characters' flag auf true
            flags.characters = 1; 
            break;
            case 'w':
            // Wenn ein -w vorliegt, setze 'words' flag auf true
            flags.words = 1; 
            break;
            case 'l':
            // wenn ein -l vorliegt, setze 'lines' flag auf true
            flags.lines = 1;
            break;
            case 'i':
            // Wenn ein -i vorliegt, und wenn das Argument eine gültige Datei ist,
            // lade diesen Dateipfad in 'file_name' hinein
            if (access(optarg, F_OK) == 0 )
                file_name = optarg;
        }
    }

    // Überprüfe ob das letzte Argumente eine lesbare Datei ist,
    // wenn mit der Option -i keine Datei übergeben wurde
    if (file_name == "")
        if (access(argv[argc-1], F_OK) == 0 )
            file_name = argv[argc-1];

    // Zeichen, Wörter oder Zeilen müssen gezählt werden ansonsten wird die Hilfeseite angezeigt
    if (flags.characters || flags.words || flags.lines) {
        if(file_name != "")
            // Weise dem Dateizeiger die Datei zu, welche sich an dem Pfad von file_name befindet
            // (die Datei wird mit Schreibrechten geöffnet)
            file = fopen(file_name, "r");

        // Variable in welcher das zu prüfende Zeichen gespeichert wird; ihr
        // wird bei der Initialisierung das ersten Zeichen der Datei zugewiesen
        int c = getc(file);

        // Iteriere jedes zeichen der Datei, bis End-of-File erreicht wurde
        while(c != EOF) {
            // erhöhe bei jedem durchlauf die Anzahl der Zeichen
            characters++;
            // erhöhe den Zeilenzäher, wenn ein Newline-Charakter gefunden wurde
            if (c == '\n') lines++;
            // erhöhe den Wortzähler, wenn ein Leerzeichen gefunden wurde
            else if (c == ' ') words++;
            // springe auf das nächste Zeichen der Datei
            c = getc(file);
        }
        // Schließe die Datei, wenn das Zählen beendet wurde
        fclose(file);

        //Wenn die 'characters' flag gesetzt wurde, gebe die Anzahl der Zeichen aus
        if(flags.characters) printf("%d characters\n", characters);
        //Wenn die 'words' flag gesetzt wurde, gebe die Anzahl der Wörter aus
        if(flags.words) printf("%d words\n", words);
        //Wenn die 'lines' flag gesetzt wurde, gebe die Anzahl der Zeilen aus
        if(flags.lines) printf("%d lines\n", lines);
    } else {
        display_usage();
        return 0;
    }
}