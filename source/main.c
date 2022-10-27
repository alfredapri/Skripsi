#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <include/curl/curl.h>

// Terminal text color
#define RESET   "\033[0m"   /* Reset color */
#define RED     "\033[31m"  /* Red for error messages */

/* Flag set by ‘--verbose’. */
static int verbose_flag;

int main (int argc, char **argv) {
    int c;

    while (1) {
        static struct option long_options[] = {
            /* These options set a flag. */
            {"verbose", no_argument,       &verbose_flag, 1},
            {"brief",   no_argument,       &verbose_flag, 0},
            /* These options don’t set a flag.
                We distinguish them by their indices. */
            {"add",     no_argument,       0, 'a'},
            {"append",  no_argument,       0, 'b'},
            {"delete",  required_argument, 0, 'd'},
            {"create",  required_argument, 0, 'c'},
            {"file",    required_argument, 0, 'f'},
            {0, 0, 0, 0}
        };
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "ab:cd:f", long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
        break;

        switch (c) {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                break;
                printf ("option %s", long_options[option_index].name);
                if (optarg)
                printf (" with arg %s", optarg);
                printf ("\n");
                break;

            case 'a':
                fprintf (stderr, RED "option -asda\n" RESET );
                break;

            case 'b':
                puts ("option -b\n");
                break;

            case 'c':
                CURL *curl;
                CURLcode res;
                char URL[1000] = "https://projectkiri.id/api";
                char strext[1000] = "?version=2&mode=searchplace&region=bdo&querystring=unpar&apikey=68CD281C8A8EE97C";

                strcat(URL, strext);
                printf("%sa\n", URL);

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
                        printf("\nPogn't\n");
                    }

                    /* always cleanup */
                    curl_easy_cleanup(curl);
                }

                curl_global_cleanup();
                break;

            case 'd':
                printf ("option -d with value `%s'\n", optarg);
                break;

            case 'f':
                // printf ("option -f with value `%s'\n", optarg);
                
                c = getopt_long (argc, argv, "a", long_options, &option_index);

                /* Detect the end of the options. */
                if (c == -1)
                break;

                switch (c) {
                    case 'a':
                    fprintf (stderr, RED "option -asda\n" RESET );
                    break;
                }
                break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                abort ();
        }
    }

    /* Instead of reporting ‘--verbose’
        and ‘--brief’ as they are encountered,
        we report the final status resulting from them. */
    if (verbose_flag) {
        puts ("verbose flag is set");
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