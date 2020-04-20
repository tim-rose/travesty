/*
 * TRAVESTY.C -- A "travesty" text generator.
 *
 * Options:
 * -o,--order <n> select travesty "order"
 * -s,--seed <n> select random seed
 * -l,--length <n> output specified No. of bytes
 *
 * Contents:
 * main()             --Process options, read corpus and output text.
 * opt_order()        --Process/validate the "order" command-line option.
 * read_corpus()      --Read the sample text to be jumbled.
 * build_candidates() --build a list of candidate follow characters.
 *
 * Remarks:
 * This program reads a sample of text and uses character
 * frequency information to garble it a little bit. By choosing
 * higher "orders" the frequency of longer sequences of characters
 * are used.
 */
#include <xtd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include <option.h>
#include <strparse.h>
#include <vector.h>
#include <estring.h>
#include <log.h>

#ifndef VERSION
#define VERSION "local"
#endif /* VERSION */
#ifndef BUILD
#define BUILD "latest"
#endif /* BUILD */

#define ORDER_MAX	16                 /* amount of lookahead */

static char *read_corpus(FILE * corpus_file, char * corpus);
static size_t build_candidates(const char *corpus, const char *prefix, char **candidate);
static int opt_order(const char *name, const char *value, void *data);

static const char name[] = "travesty";
static const char version[] = VERSION;
static const char build[] = BUILD;

/*
 * Program configuration variables controlled by command-line options.
 */
static FILE *input_file;
static size_t order;
static int random_seed;
static int chars_max;
static int print_version;

static char help_prologue[] = "travesty [options]...";
static Option opts[] = {               /* command-line options... */
    {
     .opt = 'o',.name = "order",.value_name = "value",
     .value = "3",
     .doc = "specify the travesty \"order\" (substring length)",
     .proc = opt_order,.data = (int *) &order},
    {
     .opt = 'l',.name = "length",.value_name = "characters",
     .value = "1000",
     .doc = "Output the specified No. of characters",
     .proc = opt_int,.data = (int *) &chars_max},
    {
     .opt = 's',.name = "seed",.value_name = "value",
     .value = "0",
     .doc = "initialise the random number seed value",
     .proc = opt_int,.data = (int *) &random_seed},
    {
     .opt = 'V',.name = "version",.value_name = "version",
     .doc = "print the version number and exit",
     .proc = opt_bool,.data = (int *) &print_version},
    OPTION_LOG,
    {.opt = 0}
};

/*
 * main() --Process options, read corpus and output text.
 */
int main(int argc, char *argv[])
{
    char *corpus = NEW_VECTOR(char, 0, NULL);
    char prefix[ORDER_MAX + 1] = { 0 }; /* prefix characters output */
    char *candidate = NEW_VECTOR(char, 0, NULL);
    char next_str[2] = { 0 };

    log_init(name);
    if (!opt_getopts_long(argc, argv, opts) || !opt_defaults(opts))
    {
        opt_usage(help_prologue, opts, NULL);
        exit(2);
    }
    if (print_version)
    {
        printf("travesty version %s-%s\n", version, build);
        exit(0);
    }

    random_seed = random_seed ? random_seed : time(0l);
    info("random seed: %d", random_seed);
    srand((unsigned) random_seed);

    if (optind < argc)
    {                                   /* TODO: refactor to function */
        for (; optind < argc; ++optind)
        {
            if ((input_file = fopen(argv[optind], "r")) == NULL)
            {
                log_sys(LOG_ERR, "cannot open file \"%s\"", argv[optind]);
            }
            else
            {
                corpus = read_corpus(input_file, corpus);
            }
        }
    }
    else
    {
        corpus = read_corpus(stdin, corpus);
    }

    size_t corpus_len = (size_t) vector_len(corpus);
    if (corpus_len == 0)
    {
        log_quit(1, "no input text");
    }

    corpus[corpus_len-1] = '\0';        /* terminate text */

    strncpy(prefix, corpus, order);
    for (int n = 0; n < chars_max; ++n)
    {
        size_t n_candidates = 0;

        while ((n_candidates = build_candidates(corpus, prefix, &candidate)) == 0)
        {                              /* jump to a random prefix */
            strncpy(prefix,
                    corpus + (size_t) rand() % (corpus_len - order),
                    order);
        }

        putchar(prefix[0]);             /* output and shift down */
        next_str[0] = candidate[(size_t) rand() % n_candidates];
        vstrcat(prefix, prefix + 1, next_str, NULL);
    }
    putchar('\n');
    exit(0);
}

/*
 * opt_order() --Process/validate the "order" command-line option.
 */
static int opt_order(const char *UNUSED(name), const char *text, void *data)
{
    if (!str_int_in_range(text, data, 2, ORDER_MAX))
    {
        err("order is outside allowed range: (%d-%d)", 2, ORDER_MAX);
        return 0;
    }
    return 1;
}

/*
 * read_corpus() --Read the sample text to be jumbled.
 *
 * Parameters:
 * corpus_file  --the file containing text
 * corpus_len   --returns the length of the sample text.
 *
 * Returns: (char *)
 * The sample text.
 *
 * Remarks:
 * The sample text is read into a malloc'd buffer.
 */
static char *read_corpus(FILE * corpus_file, char* corpus)
{
    char space = ' ';
    char word[30 + 1] = "";
    size_t corpus_len = 0;


    while (fscanf(corpus_file, "%30s", word) >= 1)
    {
        corpus = vector_add(corpus, strlen(word), word);
        corpus = vector_add(corpus, 1, &space);
    }
    fclose(corpus_file);

    corpus_len = (size_t) vector_len(corpus) - 1;

    info("read_corpus(): %zu characters", corpus_len);
    return (corpus);
}

/*
 * build_candidates() --build a list of candidate follow characters.
 *
 * Parameters:
 * corpus --the text to build candidates from
 * prefix --the current prefix
 * candidate --returns a vector of candidates
 *
 * Returns: (int)
 * The number of candidate follow characters for this prefix
 *
 * Remarks:
 * This routine fills the candidate string with the list of candidate
 * characters that can follow the specified prefix.
 *
 * @todo: handle empty/insufficient data in corpus
 */
static size_t build_candidates(const char *corpus, const char *prefix, char **candidates)
{
    size_t n = 0;
    char *candidates_ = *candidates;

    candidates_ = (char *) vector_delete(candidates_, 0, (size_t) vector_len(candidates_));

    for (; (corpus = strstr(corpus, prefix)) != NULL; ++corpus)
    {
        char candidate = *(corpus + order);

        if (candidate == '\0')
        {                               /* replace EOF with space */
            candidate = ' ';
        }
        candidates_ = vector_add(candidates_, 1, (void *) &candidate);
    }
    candidates_ = vector_add(candidates_, 1, (void *) empty_str);
    n = (size_t) vector_len(candidates_) - 1;
    *candidates = candidates_;
    debug("build_candidates(): prefix=\"%s\" %zu candidatess (%s)",
          prefix, n, strtrunc(candidates_, 20, NULL));
    return (n);
}
