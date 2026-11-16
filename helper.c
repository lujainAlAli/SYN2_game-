#include "scenes.h"
#include <ctype.h>
#include <stdio.h>
#include <ncurses.h>

void printChars(char *text){
    for(int i = 0; text[i] != '\0'; i++){
        addch(text[i]);
        refresh();
        napms(30);
    }
}

// scene ids start at 1, so look scenes up by id instead of array position
static Scene *find_scene(int id){
    for(int i = 0; i < scene_count; i++){
        if(scenes[i].id == id) return &scenes[i];
    }
    return NULL;
}

// ---------- status bar ----------

typedef struct {
    const char *name;   // shown to the player
    const char *file;   // text shown when the item is opened
} ItemInfo;

// one entry per Item in scenes.h
static const ItemInfo item_info[] = {
    [NEWS_ARTICLE]           = {"News article",           "items/news_article.txt"},
    [ENVIRONMENTAL_REPORT]   = {"Environmental report",   "items/environmental_report.txt"},
    [NEIGHBORHOOD_SURVEY]    = {"Neighborhood survey",    "items/neighborhood_survey.txt"},
    [SCIENTIST_NOTES]        = {"Scientist's notes",      "items/scientist_notes.txt"},
    [COUNCIL_BRIEFING]       = {"Council briefing",       "items/council_briefing.txt"},
    [COMPANY_BROCHURE]       = {"Company brochure",       "items/company_brochure.txt"},
    [MISINFORMATION_LESSON]  = {"Misinformation lesson",  "items/misinformation_lesson.txt"},
    [BIOMONITORING_PROPOSAL] = {"Biomonitoring proposal", "items/biomonitoring_proposal.txt"}
};
#define ITEM_TYPES ((int)(sizeof(item_info) / sizeof(item_info[0])))

static const char *item_name(int item){
    return (item >= 0 && item < ITEM_TYPES) ? item_info[item].name : "?";
}

// prints "Label 3" with the number green if positive, red if negative
static void print_stat(const char *label, int value){
    printw("%s ", label);
    attron(A_BOLD);
    if(value > 0)      attron(COLOR_PAIR(2));
    else if(value < 0) attron(COLOR_PAIR(1));
    printw("%d", value);
    attrset(A_NORMAL);
    printw("   ");
}

static void draw_status(Player *player){
    attron(A_BOLD);
    printw("%s", player->name);
    attrset(A_NORMAL);
    printw(" | %s   ", player->city);

    print_stat(STAT_C_NAME, player->c);
    print_stat(STAT_P_NAME, player->p);
    print_stat(STAT_K_NAME, player->k);
    printw("\n");

    if(player->pending_item > 0){
        attron(A_BOLD | COLOR_PAIR(2));
        printw("New item: %s  (press I to open it)\n", item_name(player->pending_item - 1));
        attrset(A_NORMAL);
        player->pending_item = 0;
    }

    if(player->item_count > 0){
        printw("Items: ");
        for(int i = 0; i < player->item_count; i++){
            printw("%s%s", i ? ", " : "", item_name(player->inventory[i]));
        }
        printw("\n");
    }

    for(int i = 0; i < COLS - 1; i++) addch('-');
    printw("\n\n");
}

// which text file to show: the Bayview version if there is one, else the shared one
static const char *scene_file(Scene *scene, Player *player){
    if(player->city_id == CITY_BVHP && scene->bvhp_file != NULL){
        return scene->bvhp_file;
    }
    return scene->text_file;
}

// fills visible[] with the array positions of choices the player qualifies for
static int build_menu(Scene *scene, Player *player, int visible[]){
    int n = 0;
    for(int i = 0; i < scene->choice_count; i++){
        if(can_choose(player, scene->choices[i])){
            visible[n++] = i;
        }
    }
    return n;
}

void display_scene(int scene_id, Player *player){
    char line[1024];
    clear();
    draw_status(player);

    Scene *scene = find_scene(scene_id);
    if(scene == NULL){
        printw("unknown scene %d\n", scene_id);
        refresh();
        return;
    }
    printw("[debug: scene %d, file %s]\n", scene_id, scene_file(scene, player));

    FILE *file = fopen(scene_file(scene, player), "r");
    if(file == NULL){
        printw("scene failed to load: %s\n", scene_file(scene, player));
    } else {
        while(fgets(line, sizeof(line), file) != NULL){
            print_markup(line,0);   // change to printChars when done prototyping
        }
        fclose(file);
    }

    printw("\n");
    if(scene_id == INVESTIGATION_HUB){
        printw("Leads left to follow: %d\n\n", INVESTIGATION_LIMIT - player->investigation_count);
    }
    int visible[MAX_CHOICES];
    int n = build_menu(scene, player, visible);
    for(int i = 0; i < n; i++){
        printw("[%c] %s\n", 'A' + i, scene->choices[visible[i]].text);
    }
    refresh();
}

// shows one item's text file
static void show_item(int item){
    char line[1024];
    clear();
    attron(A_BOLD);
    printw("%s\n", item_name(item));
    attrset(A_NORMAL);
    for(int i = 0; i < COLS - 1; i++) addch('-');
    printw("\n\n");

    FILE *file = fopen(item_info[item].file, "r");
    if(file == NULL){
        printw("(could not open %s)\n", item_info[item].file);
    } else {
        while(fgets(line, sizeof(line), file) != NULL){
            print_markup(line, 0);
        }
        fclose(file);
    }
    attrset(A_NORMAL);
    printw("\n\nPress any key to close.");
    refresh();
    getch();
}

// list of items; pick one to read it, Q to go back
static void open_inventory(Player *player){
    for(;;){
        clear();
        attron(A_BOLD);
        printw("ITEMS\n\n");
        attrset(A_NORMAL);
        for(int i = 0; i < player->item_count; i++){
            printw("[%c] %s\n", 'A' + i, item_name(player->inventory[i]));
        }
        printw("\n[Q] Back to the story\n");
        refresh();

        int ch = getch();
        if(ch == ERR) return;
        ch = toupper(ch);
        if(ch == 'Q') return;
        int idx = ch - 'A';
        if(idx >= 0 && idx < player->item_count){
            show_item(player->inventory[idx]);
        }
    }
}

static void print_prompt(Player *player){
    printw("\n");
    if(player->item_count > 0) printw("[I] Open an item\n");
    printw("Choose: ");
    refresh();
}

int get_scene_choice(int scene_id, Player *player){
    Scene *scene = find_scene(scene_id);
    if(scene == NULL) return -1;

    int visible[MAX_CHOICES];
    int n = build_menu(scene, player, visible);
    if(n == 0) return -1;          // ending scene (or nothing available): stop

    int ch, idx;
    print_prompt(player);
    for(;;){
        ch = getch();
        if(ch == ERR) return -1;               // input closed: stop instead of looping forever
        ch = toupper(ch);
        if(ch == 'I' && player->item_count > 0){
            open_inventory(player);
            display_scene(scene_id, player);   // redraw the scene we came from
            print_prompt(player);
            continue;
        }
        idx = ch - 'A';
        if(idx >= 0 && idx < n) break;          // valid choice letter
    }

    printw("%c\n", ch);

    Choice *picked = &scene->choices[visible[idx]];
    apply_choice(player, *picked);

    int dest = picked->dest;
    if(dest == DEST_MEETING1){
        dest = has_item(player, NEWS_ARTICLE) ? MEETING1_PREPARED : MEETING1_UNPREPARED;
    }
    // once enough investigating is done, leave the hub for good
    if(dest == INVESTIGATION_HUB && player->investigation_count >= INVESTIGATION_LIMIT){
        dest = AFTER_INVESTIGATION;
    }
    return dest;
}

// ---------- inventory ----------

void add_item(Player *player, int item){
    if(item == NO_ITEM) return;

    for(int i = 0; i < player->item_count; i++){
        if(player->inventory[i] == item) return;
    }
    if(player->item_count < MAX_ITEMS){
        player->inventory[player->item_count++] = item;
    }
}

int has_item(Player *player, int item){
    for(int i = 0; i < player->item_count; i++){
        if(player->inventory[i] == item) return 1;
    }
    return 0;
}

void apply_choice(Player *player, Choice choice){
    player->c += choice.c;
    player->p += choice.p;
    player->k += choice.k;
    if(choice.item_gain != NO_ITEM && !has_item(player, choice.item_gain)){
        add_item(player, choice.item_gain);
        player->pending_item = choice.item_gain + 1;
    }

    if(choice.inv_flag){
        player->inv_done |= choice.inv_flag;
        player->investigation_count++;
    }
}

// a min of 0 means "no requirement", so negative stats can't lock the player out
int can_choose(Player *player, Choice choice){
    if(choice.city_only && choice.city_only != player->city_id) return 0;   // other city's choice
    if(choice.inv_flag && (player->inv_done & choice.inv_flag)) return 0;   // already done
    if(choice.min_c != 0 && player->c < choice.min_c) return 0;
    if(choice.min_p != 0 && player->p < choice.min_p) return 0;
    if(choice.min_k != 0 && player->k < choice.min_k) return 0;
    return 1;
}


void init_colors(void){
    if(!has_colors()) return;
    start_color();
    use_default_colors();                 // keep the terminal's own background
    init_pair(1, COLOR_RED,     -1);
    init_pair(2, COLOR_GREEN,   -1);
    init_pair(3, COLOR_YELLOW,  -1);
    init_pair(4, COLOR_BLUE,    -1);
    init_pair(5, COLOR_MAGENTA, -1);
    init_pair(6, COLOR_CYAN,    -1);
}

#include <string.h>

static void clear_colors(void){
    for(int i = 1; i <= 6; i++) attroff(COLOR_PAIR(i));
}

// returns 1 if the tag was recognized and applied
static int apply_tag(const char *tag){
    static const char *colors[] = {"red","green","yellow","blue","magenta","cyan"};

    if(!strcmp(tag, "b"))  { attron(A_BOLD);       return 1; }
    if(!strcmp(tag, "/b")) { attroff(A_BOLD);      return 1; }
    if(!strcmp(tag, "u"))  { attron(A_UNDERLINE);  return 1; }
    if(!strcmp(tag, "/u")) { attroff(A_UNDERLINE); return 1; }
    if(!strcmp(tag, "/c")) { clear_colors();       return 1; }
    if(!strcmp(tag, "/"))  { attrset(A_NORMAL);    return 1; }   // reset everything

    for(int i = 0; i < 6; i++){
        if(!strcmp(tag, colors[i])){
            clear_colors();
            attron(COLOR_PAIR(i + 1));
            return 1;
        }
    }
    return 0;
}

// prints text, applying {tags}. delay = ms per character (0 = instant)
void print_markup(const char *s, int delay){
    for(int i = 0; s[i] != '\0'; ){
        if(s[i] == '{'){
            const char *end = strchr(s + i, '}');
            int len = end ? (int)(end - (s + i + 1)) : 0;
            if(end && len > 0 && len < 16){
                char tag[16];
                memcpy(tag, s + i + 1, len);
                tag[len] = '\0';
                if(apply_tag(tag)){
                    i += len + 2;          // skip past the closing }
                    continue;
                }
            }
        }
        // print one full character (curly quotes etc. are several bytes in UTF-8)
        int n = 1;
        while((s[i + n] & 0xC0) == 0x80) n++;
        addnstr(s + i, n);
        i += n;
        if(delay > 0){ refresh(); napms(delay); }
    }
}


// Run before initscr(): prints scene-data mistakes to the terminal.
void validate_scenes(void){
    int problems = 0;
    int reached[scene_count];
    memset(reached, 0, sizeof(reached));

    for(int i = 0; i < scene_count; i++){
        for(int j = 0; j < i; j++){
            if(scenes[i].id == scenes[j].id){
                printf("DUPLICATE id %d (entries %d and %d)\n", scenes[i].id, j, i);
                problems++;
            }
        }
        FILE *f = fopen(scenes[i].text_file, "r");
        if(f == NULL){
            printf("scene %d: missing file %s\n", scenes[i].id, scenes[i].text_file);
            problems++;
        } else fclose(f);
        if(scenes[i].bvhp_file != NULL){
            FILE *g = fopen(scenes[i].bvhp_file, "r");
            if(g == NULL){
                printf("scene %d: missing Bayview file %s\n", scenes[i].id, scenes[i].bvhp_file);
                problems++;
            } else fclose(g);
        }

        for(int k = 0; k < scenes[i].choice_count; k++){
            int d = scenes[i].choices[k].dest;
            if(d != -1 && d != DEST_MEETING1 && find_scene(d) == NULL){
                printf("scene %d, choice %c: goes to scene %d, which doesn't exist\n",
                       scenes[i].id, 'A' + k, d);
                problems++;
            }
        }
    }

    for(int i = 0; i < ITEM_TYPES; i++){
        FILE *f = fopen(item_info[i].file, "r");
        if(f == NULL){
            printf("item \"%s\": missing file %s\n", item_info[i].name, item_info[i].file);
            problems++;
        } else fclose(f);
    }

    // which scenes can the player actually reach from scene 1?
    Scene *start = find_scene(1);
    Scene *after = find_scene(AFTER_INVESTIGATION);   // reached by the hub redirect
    if(start) reached[start - scenes] = 1;
    if(after) reached[after - scenes] = 1;
    Scene *m1 = find_scene(MEETING1_PREPARED);     // reached via DEST_MEETING1
    Scene *m2 = find_scene(MEETING1_UNPREPARED);
    if(m1) reached[m1 - scenes] = 1;
    if(m2) reached[m2 - scenes] = 1;
    int changed;
    do {
        changed = 0;
        for(int i = 0; i < scene_count; i++){
            if(!reached[i]) continue;
            for(int k = 0; k < scenes[i].choice_count; k++){
                Scene *t = find_scene(scenes[i].choices[k].dest);
                if(t && !reached[t - scenes]){ reached[t - scenes] = 1; changed = 1; }
            }
        }
    } while(changed);
    for(int i = 0; i < scene_count; i++){
        if(!reached[i]){
            printf("scene %d (%s) can never be reached\n", scenes[i].id, scenes[i].title);
            problems++;
        }
    }

    if(problems){
        printf("\n%d problem(s) found. Press Enter to start anyway.", problems);
        getchar();
    }
}
