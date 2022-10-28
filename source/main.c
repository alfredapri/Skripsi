#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <include/curl/curl.h>

/* Flag set by ‘--verbose’. */
// static int verbose_flag;

int main (int argc, char **argv) {
    int mode;
    int variables;
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

        mode = getopt_long (argc, argv, "hm:", long_options, &option_index);

        /* Detect the end of the options. */
        if (mode == -1)
        break;

        switch (mode) {
            case 'h':
                // Program help
                puts ("help goes here\n");
                break;

            case 'm':
                // Program mode
                // printf ("You picked the mode: %s\n", optarg);
                if (strcmp(optarg, "search") == 0) {
                    printf ("You picked the mode: search");
                    // variables = getopt_long (argc, argv, "hm:", long_options, &option_index);
                }
                if (strcmp(optarg, "route") == 0) {
                    printf ("You picked the mode: route");
                    // variables = getopt_long (argc, argv, "hm:", long_options, &option_index);
                }
                if (strcmp(optarg, "routedirect") == 0) {
                    printf ("You picked the mode: routedirect");
                    // variables = getopt_long (argc, argv, "hm:", long_options, &option_index);
                }

                break;

            // case 'c':
            //     CURL *curl;
            //     CURLcode res;
            //     char URL[1000] = "https://projectkiri.id/api";
            //     char strext[1000] = "?version=2&mode=searchplace&region=bdo&querystring=unpar&apikey=68CD281C8A8EE97C";

            //     strcat(URL, strext);
            //     printf("%sa\n", URL);

            //     curl_global_init(CURL_GLOBAL_DEFAULT);

            //     curl = curl_easy_init();
            //     if(curl) {
            //         curl_easy_setopt(curl, CURLOPT_URL, URL);

            //         #ifdef SKIP_PEER_VERIFICATION
            //         /*
            //         * If you want to connect to a site who is not using a certificate that is
            //         * signed by one of the certs in the CA bundle you have, you can skip the
            //         * verification of the server's certificate. This makes the connection
            //         * A LOT LESS SECURE.
            //         *
            //         * If you have a CA cert for the server stored someplace else than in the
            //         * default bundle, then the CURLOPT_CAPATH option might come handy for
            //         * you.
            //         */
            //         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            //         #endif

            //         #ifdef SKIP_HOSTNAME_VERIFICATION
            //         /*
            //         * If the site you are connecting to uses a different host name that what
            //         * they have mentioned in their server certificate's commonName (or
            //         * subjectAltName) fields, libcurl will refuse to connect. You can skip
            //         * this check, but this will make the connection less secure.
            //         */
            //         curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            //         #endif

            //         /* Perform the request, res will get the return code */
            //         res = curl_easy_perform(curl);
            //         /* Check for errors */
            //         if(res != CURLE_OK) {
            //             fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            //             printf("\nPogn't\n");
            //         }

            //         /* always cleanup */
            //         curl_easy_cleanup(curl);
            //     }

            //     curl_global_cleanup();
            //     break;

            // case 'd':
            //     printf ("option -d with value `%s'\n", optarg);
            //     break;

            // case 'f':
            //     // printf ("option -f with value `%s'\n", optarg);
                
            //     c = getopt_long (argc, argv, "a", long_options, &option_index);

            //     /* Detect the end of the options. */
            //     if (c == -1)
            //     break;

            //     switch (c) {
            //         case 'a':
            //         fprintf (stderr, "option -asda\n");
            //         break;
            //     }
            //     break;

            case '?':
                /* getopt_long already printed an error message. */
                puts ("invalid option message goes here\n");
                break;

            default:
                abort ();
        }
    }

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
        printf ("%s ", argv[optind++]);
        putchar ('\n');
    }

    exit (0);
}