#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <include/curl/curl.h>

int mode; // 0 = undefined, 1 = help, 2 = searchplace, 3 = findroute, 4 = direct
int region; // 0 = undefined, 1 = cgk, 2 = bdo, 3 = mlg, 4 = sub
char query[100];
char start[100];
char finish[100];
int regstart; // 0 = undefined, 1 = cgk, 2 = bdo, 3 = mlg, 4 = sub
int regfinish; // 0 = undefined, 1 = cgk, 2 = bdo, 3 = mlg, 4 = sub
int locale; // 0 = id, 1 = en, 2 = invalid

int main (int argc, char **argv) {
    int funct;
    extern int opterr;
    opterr = 0;

    while (1) {
        static struct option long_options[] = {
            {"help",        no_argument,        0,  'h'},
            {"mode",        required_argument,  0,  'm'},
            {"region",      required_argument,  0,  'r'},
            {"query",       required_argument,  0,  'q'},
            {"start",       required_argument,  0,  's'},
            {"finish",      required_argument,  0,  'f'},
            {"regstart",    required_argument,  0,  'S'},
            {"regfinish",   required_argument,  0,  'F'},
            {"locale",      required_argument,  0,  'l'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        funct = getopt_long (argc, argv, ":hm:r:q:s:S:f:F:l:", long_options, &option_index);

        /* Detect the end of the options. */
        if (funct == -1) {
            break;
        }

        // Parse all inputs
        switch (funct) {
            case 'h':
                // Program help
                mode = 1;
                break;

            case 'm':
                // Program mode
                if (strcmp(optarg, "searchplace") == 0) {
                    mode = 2;
                }
                else if (strcmp(optarg, "findroute") == 0) {
                    mode = 3;
                }
                else if (strcmp(optarg, "direct") == 0) {
                    mode = 4;
                }
                // else {
                //     puts("Anda telah memasukkan mode yang tidak valid.");
                //     puts("Mohon periksa kembali apakah mode yang anda masukkan sudah diketik dengan benar.");
                //     abort ();
                // }
                break;
            
            case 'r':
                // Location regions
                if (strcmp(optarg, "cgk") == 0) {
                    region = 1;
                }
                else if (strcmp(optarg, "bdo") == 0) {
                    region = 2;
                }
                else if (strcmp(optarg, "mlg") == 0) {
                    region = 3;
                }
                else if (strcmp(optarg, "sub") == 0) {
                    region = 4;
                }
                // else {
                    // puts("Anda telah memasukkan region yang tidak valid.");
                    // puts("Mohon periksa kembali apakah kode region yang anda masukkan merupakan salah satu dari empat kode yang tersedia.");
                    // abort ();
                // }
                break;

            case 'q':
                // General location search keyword
                if (optarg[0] == '\0') {
                    // puts("Anda telah memasukkan kata kunci yang tidak valid.");
                    // puts("Mohon periksa kembali apakah kode region yang anda masukkan merupakan salah satu dari empat kode yang tersedia.");
                    // abort ();
                }
                else {
                    strcpy(query, optarg);
                }
                break;
            
            case 's':
                // General location search keyword
                if (optarg[0] == '\0') {
                    // puts("Anda telah memasukkan kata kunci yang tidak valid.");
                    // puts("Mohon periksa kembali apakah kode region yang anda masukkan merupakan salah satu dari empat kode yang tersedia.");
                    // abort ();
                }
                else {
                    strcpy(start, optarg);
                }
                break;
            
            case 'f':
                // General location search keyword
                if (optarg[0] == '\0') {
                    // puts("Anda telah memasukkan kata kunci yang tidak valid.");
                    // puts("Mohon periksa kembali apakah kode region yang anda masukkan merupakan salah satu dari empat kode yang tersedia.");
                    // abort ();
                }
                else {
                    strcpy(finish, optarg);
                }
                break;
            
            case 'S':
                // Starting location regions
                if (strcmp(optarg, "cgk") == 0) {
                    regstart = 1;
                }
                else if (strcmp(optarg, "bdo") == 0) {
                    regstart = 2;
                }
                else if (strcmp(optarg, "mlg") == 0) {
                    regstart = 3;
                }
                else if (strcmp(optarg, "sub") == 0) {
                    regstart = 4;
                }
                break;
            
            case 'F':
                // Finish location regions
                if (strcmp(optarg, "cgk") == 0) {
                    regfinish = 1;
                }
                else if (strcmp(optarg, "bdo") == 0) {
                    regfinish = 2;
                }
                else if (strcmp(optarg, "mlg") == 0) {
                    regfinish = 3;
                }
                else if (strcmp(optarg, "sub") == 0) {
                    regfinish = 4;
                }
                break;
            
            case 'l':
                // Locale
                if (strcmp(optarg, "id") == 0) {
                    locale = 0;
                }
                else if (strcmp(optarg, "en") == 0) {
                    locale = 1;
                }
                else {
                    locale = 2;
                }
                break;

            case ':':
                // Error: missing arguments
                puts("Salah satu dari opsi yang anda masukkan kehilangan argumen yang dibutuhkan.");
                puts("Mohon periksa kembali penulisan perintah yang anda masukkan.");
                abort();

            case '?':
                // Error: unknown options
                puts("Anda telah memasukkan opsi yang tidak valid.");
                puts("Mohon periksa kembali penulisan perintah yang anda masukkan.");
                abort();

            default:
                abort();
        }
    }

    // Error: extra arguments
    if (optind < argc) {
        printf("Anda telah memasukkan kelebihan argumen: ");
        while (optind < argc) {
            printf("%s ", argv[optind++]);
        }
        putchar('\n');
        puts("Mohon periksa kembali penulisan perintah yang anda masukkan.");
    }
    else {
        // process goes here
    }

    exit (0);
}