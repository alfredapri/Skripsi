#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <cJSON/cJSON.h>
#include "curl/curl.h"

cJSON *responseJSON;
char URL[1000] = "https://projectkiri.id/api?version=2";
int mode = -1; // -1 = undefined, 0 = unknown, 1 = help, 2 = searchplace, 3 = findroute, 4 = direct
int region = -1; // -1 = undefined, 0 = unknown, 1 = cgk, 2 = bdo, 3 = mlg, 4 = sub
char query[100];
char start[100];
char finish[100];
char escape[100]; // Temporary variable for escaping string inputs
int regstart = -1; // -1 = undefined, 0 = unknown, 1 = cgk, 2 = bdo, 3 = mlg, 4 = sub
int regfinish = -1; // -1 = undefined, 0 = unknown, 1 = cgk, 2 = bdo, 3 = mlg, 4 = sub
int locale = 0; // 0 = id, 1 = en, 2 = unknown
int step; // 0 = search starting location, 1 = search finish location, 2 = find route

size_t write_searchplace(void *data, size_t size, size_t nmemb, void *userdata) {
    // Function must return realsize
    // This variable served no purpose nor is it needed to check for memory usage
    // since the response size will always be smaller than the maximum allowed memory buffer
    size_t realsize = size * nmemb;

    cJSON *status;
    cJSON *result;
    cJSON *resultitem;
    cJSON *resultitemname;
    cJSON *resultitemlocation;
    int indexitem;

    responseJSON = cJSON_Parse(data);
    status = cJSON_GetObjectItem(responseJSON, "status");
    
    // Check whether API returned an error
    if (strcmp(status->valuestring, "ok") != 0) {
        result = cJSON_GetObjectItem(responseJSON, "message");
        printf("%s\n", result->valuestring);
    }
    else {
        result = cJSON_GetObjectItem(responseJSON, "searchresult");

        // Check whether API managed to find a single result
        if (cJSON_GetArraySize(result) == 0) {
            if (locale == 1) {
                fputs("Location not found.\n", stderr);
                fputs("Please recheck whether the search keyword was correct.\n", stderr);
            }
            else {
                fputs("Lokasi tidak berhasil ditemukan.\n", stderr);
                fputs("Silahkan cek ulang apakah kata kunci pencarian sudah benar.\n", stderr);
            }
            puts("====================");
        }
        else {
            indexitem = 1;

            puts("====================");
            cJSON_ArrayForEach(resultitem, result) {
                resultitemname = cJSON_GetObjectItem(resultitem, "placename");
                resultitemlocation = cJSON_GetObjectItem(resultitem, "location");

                // Print location index only if there were more than one found
                if (cJSON_GetArraySize(result) > 1) {
                    if (locale == 1) {
                        printf("Location %d:\n", indexitem);
                    }
                    else printf("Lokasi %d:\n", indexitem);
                }
                if (locale == 1) {
                    printf("Name: %s\n", resultitemname->valuestring);
                    printf("Coordinates: %s\n", resultitemlocation->valuestring);
                }
                else {
                    printf("Nama: %s\n", resultitemname->valuestring);
                    printf("Koordinat: %s\n", resultitemlocation->valuestring);
                }
                puts("====================");
                indexitem++;
            }
        }
    }

    return realsize;
}

size_t write_searchplace_noreturns(void *data, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;

    cJSON *status;
    cJSON *result;
    cJSON *resultitem;
    cJSON *resultitemname;
    cJSON *resultitemlocation;

    responseJSON = cJSON_Parse(data);
    status = cJSON_GetObjectItem(responseJSON, "status");
    
    // Check whether API returned an error
    if (strcmp(status->valuestring, "ok") != 0) {
        result = cJSON_GetObjectItem(responseJSON, "message");
        printf("%s\n", result->valuestring);
    }
    else {
        result = cJSON_GetObjectItem(responseJSON, "searchresult");

        // Check whether API managed to find a single result
        if (cJSON_GetArraySize(result) == 0) {
            if (step == 0) {
                if (locale == 1) {
                    fputs("Starting location not found.\n", stderr);
                    fputs("Please recheck whether the search keyword was correct.\n", stderr);
                }
                else {
                    fputs("Lokasi awal tidak berhasil ditemukan.\n", stderr);
                    fputs("Silahkan cek ulang apakah kata kunci pencarian sudah benar.\n", stderr);
                }
            }
            else if (step == 1) {
                if (locale == 1) {
                    fputs("Finish location not found.\n", stderr);
                    fputs("Please recheck whether the search keyword was correct.\n", stderr);
                }
                else {
                    fputs("Lokasi akhir tidak berhasil ditemukan.\n", stderr);
                    fputs("Silahkan cek ulang apakah kata kunci pencarian sudah benar.\n", stderr);
                }
            }
        }
        else {
            // Direct route mode only supports first location found
            resultitem = cJSON_GetArrayItem(result, 0);
            resultitemname = cJSON_GetObjectItem(resultitem, "placename");
            resultitemlocation = cJSON_GetObjectItem(resultitem, "location");

            if (step == 0) {
                if (locale == 1) {
                    printf("Starting location: %s\n", resultitemname->valuestring);
                }
                else printf("Lokasi awal: %s\n", resultitemname->valuestring);
                strcpy(start, resultitemlocation->valuestring);
            }
            else if (step == 1) {
                if (locale == 1) {
                    printf("Finish location: %s\n", resultitemname->valuestring);
                }
                else printf("Lokasi akhir: %s\n", resultitemname->valuestring);
                strcpy(finish, resultitemlocation->valuestring);
            }
        }
    }

    return realsize;
}

size_t write_findroute(void *data, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;

    cJSON *status;
    cJSON *result;
    cJSON *route;
    cJSON *routesteps;
    cJSON *routetime;
    cJSON *routestepitem;
    cJSON *routestepdetail;
    int indexroute;
    int indexstep;

    responseJSON = cJSON_Parse(data);
    status = cJSON_GetObjectItem(responseJSON, "status");

    // Check whether API returned an error
    if (strcmp(status->valuestring, "ok") != 0) {
        result = cJSON_GetObjectItem(responseJSON, "message");
        printf("%s\n", result->valuestring);
    }
    else {
        result = cJSON_GetObjectItem(responseJSON, "routingresults");
        indexroute = 1;

        // For each possible routes...
        cJSON_ArrayForEach(route, result) {
            routesteps = cJSON_GetObjectItem(route, "steps");
            routetime = cJSON_GetObjectItem(route, "traveltime");

            // Print route index only if there were more than one found
            if (cJSON_GetArraySize(result) > 1) {
                if (indexroute > 1) {
                    putchar('\n');
                }
                puts("====================");

                if (locale == 1) {
                    printf("Route %d:\n", indexroute);
                }
                else printf("Rute %d:\n", indexroute);
            }

            // Check whether API managed to find a single result
            if (cJSON_IsNull(routetime)) {
                if (locale == 1) {
                    puts("Sorry, we are unable to find a route for you.");
                }
                else puts("Maaf, rute tidak berhasil ditemukan.");
                puts("====================");
            }
            else {
                indexstep = 1;
                
                if (locale == 1) {
                    printf("Estimated duration: %s\n", routetime->valuestring);
                }
                else printf("Estimasi waktu: %s\n", routetime->valuestring);
                puts("====================");
                
                cJSON_ArrayForEach(routestepitem, routesteps) {
                    routestepdetail = cJSON_GetArrayItem(routestepitem, 3);

                    if (locale == 1) {
                        printf("Step %d: ", indexstep);
                    }
                    else printf("Langkah %d: ", indexstep);
                    printf("%s\n", routestepdetail->valuestring);
                    indexstep++;
                }
                puts("====================");
            }
            indexroute++;
        }
    }

    return realsize;
}

void print_help() {
    puts("KIRI Command Line Tool, version x.x.x");
    puts("Use the KIRI tool through the command line.");
    putchar('\n');
    puts("USAGE:");
    puts("    ./kiritool [COMMAND] [OPTIONS]");
    putchar('\n');
    puts("COMMAND:");
    puts("    -h, --help          Display usage tutorial.");
    puts("    -m, --mode <MODE>   Set the tool operation mode.\n");
    puts("    You can only choose one mode per operation.");
    putchar('\n');
    puts("OPTIONS:");
    puts("    -F, --regfinish <REGION>    Set region to search for finish location in.");
    puts("    -f, --finish <FINISH>       Set finish location.");
    puts("    -l, --locale <LANG>         Set tool output language.");
    puts("    -S, --regstart <REGION>     Set region to search for starting location in.");
    puts("    -s, --start <START>         Set starting location.");
    puts("    -q, --query <KEYWORD>       Set keyword for searching.");
    puts("    -r, --region <REGION>       Set region to search for location in.");
    putchar('\n');
    puts("ARGUMENTS:");
    puts("    <FINISH>");
    puts("        Route finish location.");
    puts("        For 'findroute' mode, input the location's latitude and longitude coordinates.");
    puts("        For 'direct' mode, input the location's search keyword (<KEYWORD> argument).");
    puts("    <KEYWORD>");
    puts("        Keywords used by the tool to search for the desired location.");
    puts("        For multiple words queries, encase the keywords with apostrophes (\" \").");
    puts("    <LANG>");
    puts("        Tool output language.");
    puts("        Available languages: id, en");
    puts("          - id (Indonesian - Default)");
    puts("          - en (English)");
    puts("    <MODE>");
    puts("        Tool operation mode. Note that not all arguments are needed for all modes.");
    puts("        Should the user input extra arguments, they will simply not be used.");
    puts("        Available modes:");
    puts("          - searchplace");   
    puts("                Required arguments: --query, --region");
    puts("                Optional arguments: --locale");
    puts("          - findroute");   
    puts("                Required arguments: --start, --finish");
    puts("                Optional arguments: --locale");
    puts("          - findroute");   
    puts("                Required arguments: --regstart, --start, --regfinish, --finish");
    puts("                Optional arguments: --locale");
    puts("    <REGION>");
    puts("        Region to search for locations in.");
    puts("        Available regions:");
    puts("          - cgk (Jakarta)");
    puts("          - bdo (Bandung)");
    puts("          - mlg (Malang)");
    puts("          - sub (Surabaya)");
    puts("    <START>");
    puts("        Route starting location.");
    puts("        For 'findroute' mode, input the location's latitude and longitude coordinates.");
    puts("        For 'direct' mode, input the location's search keyword (<KEYWORD> argument).");
    putchar('\n');
}

void execute_curl() {
    CURL *curl;
    CURLcode response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        switch (mode) {
            case 2: // searchplace
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_searchplace);
                response = curl_easy_perform(curl);
                // Check CURL response code for errors
                if (response != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(response));
                }
                break;
            
            case 3: // findroute
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_findroute);
                response = curl_easy_perform(curl);
                if (response != CURLE_OK) {
                    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(response));
                }
                break;

            case 4: // directroute
                if (step == 0) {
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_searchplace_noreturns);
                    response = curl_easy_perform(curl);
                }
                else if (step == 1) {
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_searchplace_noreturns);
                    response = curl_easy_perform(curl);
                    if (response != CURLE_OK) {
                        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(response));
                    }
                }
                else if (step == 2) {
                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_findroute);
                    response = curl_easy_perform(curl);
                    if (response != CURLE_OK) {
                        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(response));
                    }
                }
                break;

            default:
                break;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void build_url_searchplace(int region, char* query) {
    strcat(URL, "&mode=searchplace");

    // Region check
    switch (region) {
        case -1:
            fputs("Fitur pencarian lokasi memerlukan pengaturan region lokasi yang ingin dicari.\n", stderr);
            fputs("Mohon pastikan anda sudah memasukkan salah satu dari empat kode region yang tersedia.\n", stderr);
            fputs("Pilihan region: cgk, bdo, mlg, sub\n", stderr);
            abort();
            break;
        
        case 1:
            strcat(URL, "&region=cgk");
            break;
        
        case 2:
            strcat(URL, "&region=bdo");
            break;
        
        case 3:
            strcat(URL, "&region=mlg");
            break;
        
        case 4:
            strcat(URL, "&region=sub");
            break;

        default:
            fputs("Anda telah memasukkan region yang tidak valid.\n", stderr);
            fputs("Mohon periksa kembali apakah kode region yang anda masukkan merupakan salah satu dari empat kode region yang tersedia.\n", stderr);
            fputs("Pilihan region: cgk, bdo, mlg, sub\n", stderr);
            abort();
            break;
    }

    // Query check
    if (strcmp(query, "\0") == 0) {
        fputs("Fitur pencarian lokasi memerlukan sebuah kata kunci pencarian.\n", stderr);
        fputs("Mohon pastikan anda sudah memasukkan kata kunci untuk melakukan pencarian lokasi.\n", stderr);
        abort();
    }
    else {
        strcat(URL, "&querystring=");
        strcat(URL, query);
    }
    strcat(URL, "&apikey=68CD281C8A8EE97C");
}

void build_url_findroute(int locale, char* start, char* finish) {
    strcat(URL, "&mode=findroute");

    // Locale check
    switch (locale) {
        case 1:
            strcat(URL, "&locale=en");
            break;
        
        case 2:
            fputs("Anda telah memasukkan pilihan bahasa (locale) yang tidak valid.\n", stderr);
            fputs("Mohon periksa kembali apakah pilihan bahasa yang anda masukkan valid atau tidak.\n", stderr);
            fputs("Pilihan locale: id, en\n", stderr);
            abort();
            break;
        
        default:
            strcat(URL, "&locale=id");
            break;
    }

    // Starting location check
    if (strcmp(start, "\0") == 0) {
        fputs("Anda belum memasukkan sebuah koordinat untuk lokasi awal.\n", stderr);
        fputs("Mohon masukkan koordinat untuk lokasi awal pencarian rute melalui opsi yang sesuai.\n", stderr);
        abort();
    }
    else {
        strcat(URL, "&start=");
        strcat(URL, start);
    }

    // End location check
    if (strcmp(finish, "\0") == 0) {
        fputs("Anda belum memasukkan sebuah koordinat untuk lokasi akhir.\n", stderr);
        fputs("Mohon masukkan koordinat untuk lokasi akhir pencarian rute melalui opsi yang sesuai.\n", stderr);
        abort();
    }
    else {
        strcat(URL, "&finish=");
        strcat(URL, finish);
    }

    strcat(URL, "&presentation=desktop");
    strcat(URL, "&apikey=68CD281C8A8EE97C");
}

void reset_url() {
    // Reset URL for multiple API processes at once
    strcpy(URL, "https://projectkiri.id/api?version=2");
}

void replace_space(char* string) {
    // Escapes all whitespace characters in string
    int i;
    int j = 0;

    for (i = 0; i < 100; i++) {
        // printf("%c ", string[i]);
        if (string[i] == '\0') {
            break;
        }

        if (string[i] == ' ') {
            escape[j] = '%';
            escape[j + 1] = '2';
            escape[j + 2] = '0';
            j = j + 2;
        }
        else escape[j] = string[i];

        j++;
    }
}

int main(int argc, char **argv) {
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
                else {
                    mode = 0;
                }
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
                else {
                    region = 0;
                }
                break;

            case 'q':
                // General location search keyword
                replace_space(optarg);
                strcpy(query, escape);
                memset(escape, 0, sizeof(escape));
                break;
            
            case 's':
                // General location search keyword
                if (optarg[0] == '\0') {
                    // puts("Anda telah memasukkan kata kunci yang tidak valid.");
                    // puts("Mohon periksa kembali apakah kode region yang anda masukkan merupakan salah satu dari empat kode yang tersedia.");
                    // abort();
                }
                else {
                    replace_space(optarg);
                    strcpy(start, escape);
                    memset(escape, 0, sizeof(escape));
                }
                break;
            
            case 'f':
                // General location search keyword
                if (optarg[0] == '\0') {
                    // puts("Anda telah memasukkan kata kunci yang tidak valid.");
                    // puts("Mohon periksa kembali apakah kode region yang anda masukkan merupakan salah satu dari empat kode yang tersedia.");
                    // abort();
                }
                else {
                    replace_space(optarg);
                    strcpy(finish, escape);
                    memset(escape, 0, sizeof(escape));
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
                else {
                    regstart = 0;
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
                else {
                    regfinish = 0;
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
                fputs("Salah satu dari opsi yang anda masukkan kehilangan argumen yang dibutuhkan.\n", stderr);
                fputs("Mohon periksa kembali penulisan perintah yang anda masukkan.\n", stderr);
                abort();

            case '?':
                // Error: unknown options
                fputs("Anda telah memasukkan opsi yang tidak valid.\n", stderr);
                fputs("Mohon periksa kembali penulisan perintah yang anda masukkan.\n", stderr);
                abort();

            default:
                abort();
        }
    }

    // Error: extra arguments
    if (optind < argc) {
        fprintf(stderr, "Anda telah memasukkan kelebihan argumen: ");
        while (optind < argc) {
            fprintf(stderr, "%s ", argv[optind++]);
        }
        fputs("\nMohon periksa kembali penulisan perintah yang anda masukkan.\n", stderr);
    }
    else {
        switch (mode) {
            case -1:
                fputs("Mohon masukkan mode pengunaan perkakas.\n", stderr);
                abort();
                break;

            case 1:
                print_help();
                break;
            
            case 2:
                build_url_searchplace(region, query);
                // printf("%s\n", URL);
                execute_curl();
                break;
            
            case 3:
                build_url_findroute(locale, start, finish);
                // printf("%s\n", URL);
                execute_curl();
                break;
            
            case 4:
                step = 0;
                build_url_searchplace(regstart, start);
                // printf("%s\n", URL);
                execute_curl();
                reset_url();
                step = 1;
                build_url_searchplace(regstart, finish);
                // printf("%s\n", URL);
                execute_curl();
                reset_url();
                step = 2;
                build_url_findroute(locale, start, finish);
                // printf("%s\n", URL);
                execute_curl();
                break;

            default:
                fputs("Anda telah memasukkan mode yang tidak valid.\n", stderr);
                fputs("Mohon periksa kembali apakah mode yang anda masukkan sudah diketik dengan benar.\n", stderr);
                abort();
                break;
        }
    }

    exit (0);
}