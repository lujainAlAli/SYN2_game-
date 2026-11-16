#ifndef SCENES_H
#define SCENES_H

#define MAX_CHOICES 8

// names shown for the three stats (change these to rename them everywhere)
#define STAT_C_NAME "Community"
#define STAT_P_NAME "Power"
#define STAT_K_NAME "Knowledge"

#define CITY_SANTA_CLARA 1
#define CITY_BVHP        2
#define MAX_ITEMS 10
#define NO_ITEM -1

// investigation hub
#define INVESTIGATION_HUB    21
#define INVESTIGATION_LIMIT  3
#define AFTER_INVESTIGATION  30

// special destination: first public meeting. Picks 10 if the player read
// the news article (prepared), otherwise 11.
#define DEST_MEETING1        -2
#define MEETING1_PREPARED    10
#define MEETING1_UNPREPARED  11

typedef enum {
    NEWS_ARTICLE,
    ENVIRONMENTAL_REPORT,
    NEIGHBORHOOD_SURVEY,
    SCIENTIST_NOTES,
    COUNCIL_BRIEFING,
    COMPANY_BROCHURE,
    MISINFORMATION_LESSON,
    BIOMONITORING_PROPOSAL
} Item;

typedef struct {
    char text[100];
    int dest;               // id of the next scene (-1 = end game)

    // Score changes
    int c, p, k;

    // Minimum scores required (0 = no requirement)
    int min_c, min_p, min_k;

    // Item awarded (NO_ITEM = none)
    int item_gain;

    // nonzero = an investigation activity (one bit per activity, pick once)
    unsigned int inv_flag;

    // 0 = shown to everyone, otherwise only to that city (CITY_...)
    int city_only;
} Choice;

typedef struct {
    int id;
    char title[50];
    char *text_file;
    char *graphic_file;
    Choice choices[MAX_CHOICES];
    int choice_count;
    char *bvhp_file;   // optional: text file for Bayview Hunters Point players
} Scene;

typedef struct {
    char name[50];
    char city[50];
    int pending_item;   // item just received (item id + 1, 0 = none), shown once
    int city_id;        // CITY_SANTA_CLARA or CITY_BVHP

    int c, p, k;

    int inventory[MAX_ITEMS];
    int item_count;

    int read_article;
    int met_neighbor;
    int prepared;
    int investigation_count;
    unsigned int inv_done;      // which investigation activities are finished
    int meeting_approach;
} Player;

extern Scene scenes[];
extern int scene_count;

// helper.c
void printChars(char *text);
void display_scene(int scene_id, Player *player);
int  get_scene_choice(int scene_id, Player *player);
void add_item(Player *player, int item);
int  has_item(Player *player, int item);
void apply_choice(Player *player, Choice choice);
int  can_choose(Player *player, Choice choice);
void init_colors(void);
void validate_scenes(void);
void print_markup(const char *s, int delay);
#endif
