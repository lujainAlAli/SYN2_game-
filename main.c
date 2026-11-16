#include "compatibility.h"
#include <string.h>
#include "scenes.h"

void init_game(Player *player);

int main(){
    validate_scenes();
    initscr();
    init_colors();
    cbreak();
    echo();

    Player player;
    init_game(&player);

    int current_scene = 1;          // first scene id
    while(current_scene != -1){
        display_scene(current_scene, &player);
        current_scene = get_scene_choice(current_scene, &player);
    }

    printw("\n\nThe End. Press any key.");
    refresh();
    getch();
    endwin();
    return 0;
}

void get_input(char *dest_str, int limit){
    getnstr(dest_str, limit - 1);
}

void init_game(Player *player){
    memset(player, 0, sizeof(*player));   // zeroes stats, inventory, item_count, flags

    echo();
    printChars("Enter Name:\n");
    get_input(player->name, 50);
    printw("Hello, %s\n\n", player->name);
    printw("Pick a location:\n[1] Santa Clara\n[2] Bayview Hunters Point\n");
    refresh();

    int city;
    do {
        city = getch();
    } while(city != '1' && city != '2');

    if(city == '1'){
        strcpy(player->city, "Santa Clara");
        player->city_id = CITY_SANTA_CLARA;
    } else {
        strcpy(player->city, "Bayview Hunters Point");
        player->city_id = CITY_BVHP;
    }

    noecho();
    clear();
}
