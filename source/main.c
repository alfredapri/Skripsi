#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <include/curl/curl.h>

char URL[1000] = "https://projectkiri.id/api?version=2";
int mode = -1; // -1 = undefined, 0 = unknown, 1 = help, 2 = searchplace, 3 = findroute, 4 = direct
int region = -1; // -1 = undefined, 0 = unknown, 1 = cgk, 2 = bdo, 3 = mlg, 4 = sub
char query[100];
char start[100];
char finish[100];
int regstart = -1; // -1 = undefined, 0 = unknown, 1 = cgk, 2 = bdo, 3 = mlg, 4 = sub
int regfinish = -1; // -1 = undefined, 0 = unknown, 1 = cgk, 2 = bdo, 3 = mlg, 4 = sub
int locale; // 0 = id, 1 = en, 2 = unknown

void execute_curl() {
    // Send inputs to API
    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, URL);

        #ifdef SKIP_PEER_VERIFICATION
        /*
        * If you want to connect to a site who is not using a certificate that is
        * signed by one of the certs in the CA bundle you have, you can skip the
        * verification of the server's certificate. This makes the connection
        * A LOT LESS SECURE.
        *
        * If you have a CA cert for the server stored someplace else than in the
        * default bundle, then the CURLOPT_CAPATH option might come handy for
        * you.
        */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        #endif

        #ifdef SKIP_HOSTNAME_VERIFICATION
        /*
        * If the site you are connecting to uses a different host name that what
        * they have mentioned in their server certificate's commonName (or
        * subjectAltName) fields, libcurl will refuse to connect. You can skip
        * this check, but this will make the connection less secure.
        */
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        #endif

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void build_url_searchplace() {
    strcat(URL, "&mode=searchplace");

    // Region check
    switch (region) {
        case -1:
            puts("Fitur pencarian lokasi memerlukan pengaturan region lokasi yang ingin dicari.");
            puts("Mohon pastikan anda sudah memasukkan salah satu dari empat kode region yang tersedia.");
            puts("Pilihan region: cgk, bdo, mlg, sub");
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
            puts("Anda telah memasukkan region yang tidak valid.");
            puts("Mohon periksa kembali apakah kode region yang anda masukkan merupakan salah satu dari empat kode region yang tersedia.");
            puts("Pilihan region: cgk, bdo, mlg, sub");
            abort();
            break;
    }

    // Query check
    if (strcmp(query, "\0") == 0) {
        puts("Fitur pencarian lokasi memerlukan sebuah kata kunci pencarian.");
        puts("Mohon pastikan anda sudah memasukkan kata kunci untuk melakukan pencarian lokasi.");
        abort();
    }
    else {
        strcat(URL, "&querystring=");
        strcat(URL, query);
    }
}

void build_url_findroute() {
    URL = "https://projectkiri.id/api?version=2";
    strcat(URL, "&mode=findroute");

    // Locale check
    switch (locale) {
        case 1:
            strcat(URL, "&locale=en");
            break;
        
        case 2:
            puts("Anda telah memasukkan pilihan bahasa (locale) yang tidak valid.");
            puts("Mohon periksa kembali apakah pilihan bahasa yang anda masukkan valid atau tidak.");
            puts("Pilihan locale: id, en");
            abort();
            break;
        
        default:
            strcat(URL, "&locale=id");
            break;
    }

    // Starting location check
    if (strcmp(start, "\0") == 0) {
        puts("Anda belum memasukkan sebuah koordinat untuk lokasi awal.");
        puts("Mohon masukkan koordinat untuk lokasi awal pencarian rute melalui opsi yang sesuai.");
        abort();
    }
    else {
        strcat(URL, "&start=");
        strcat(URL, start);
    }

    // End location check
    if (strcmp(finish, "\0") == 0) {
        puts("Anda belum memasukkan sebuah koordinat untuk lokasi akhir.");
        puts("Mohon masukkan koordinat untuk lokasi akhir pencarian rute melalui opsi yang sesuai.");
        abort();
    }
    else {
        strcat(URL, "&finish=");
        strcat(URL, finish);
    }

    strcat(URL, "&presentation=desktop");
}

void build_url_directroute() {
    
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
                strcpy(query, optarg);
                break;
            
            case 's':
                // General location search keyword
                if (optarg[0] == '\0') {
                    // puts("Anda telah memasukkan kata kunci yang tidak valid.");
                    // puts("Mohon periksa kembali apakah kode region yang anda masukkan merupakan salah satu dari empat kode yang tersedia.");
                    // abort();
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
                    // abort();
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
        if (mode == -1) {
            puts("Mohon masukkan mode pengunaan perkakas.");
            abort();
        }
        if (mode == 1) {
            // help goes here
        }
        else if (mode == 2) {
            build_url_searchplace();
        }
        else if (mode == 3) {
            build_url_findroute();
        }
        else if (mode == 4) {
            
        }
        else {
            puts("Anda telah memasukkan mode yang tidak valid.");
            puts("Mohon periksa kembali apakah mode yang anda masukkan sudah diketik dengan benar.");
            abort();
        }

        // Append API key
        strcat(URL, "&apikey=68CD281C8A8EE97C");

        printf("%s", URL);

        // execute_curl();
    }

    exit (0);
}