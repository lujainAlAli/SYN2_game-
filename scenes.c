#include "scenes.h"
#include <stddef.h>

#define CHOICE(t, d, dc, dp, dk, mc, mp, mk, item) \
    {t, d, dc, dp, dk, mc, mp, mk, item}

// an investigation activity: can be picked once, counts toward the limit
#define INV_CHOICE(t, d, dc, dp, dk, item, flag) \
    {t, d, dc, dp, dk, 0, 0, 0, item, flag}

// same, but only shown to one city (CITY_SANTA_CLARA or CITY_BVHP)
#define INV_CHOICE_CITY(t, d, dc, dp, dk, item, flag, city) \
    {t, d, dc, dp, dk, 0, 0, 0, item, flag, city}

// Scene id N loads storyline/N.txt. If a scene has a last field like
// "storyline/105.txt", Bayview Hunters Point players get that file instead.
Scene scenes[] = {

    // SCENE 1: Moving in
    {
        1, "Moving in", "storyline/1.txt", NULL,
        {
            CHOICE("Open and read the news article",
                2, 0, 0, 1, 0, 0, 0, NEWS_ARTICLE),
            CHOICE("Scroll past the news",
                3, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        2,
        "storyline/100.txt"
    },

    // SCENE 2: Read the article
    {
        2, "Read the article", "storyline/2.txt", NULL,
        {
            CHOICE("Take breakfast outside",
                4, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1,
        "storyline/101.txt"
    },

    // SCENE 3: Scrolled past
    {
        3, "Scrolled past", "storyline/3.txt", NULL,
        {
            CHOICE("Take breakfast outside",
                4, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1,
        "storyline/102.txt"
    },

    // SCENE 4: Breakfast
    {
        4, "Breakfast", "storyline/4.txt", NULL,
        {
            CHOICE("Wave back",
                5, 1, 1, 1, 0, 0, 0, NO_ITEM),
            CHOICE("Ignore the neighbor",
                7, -1, -1, 0, 0, 0, 0, NO_ITEM)
        },
        2,
        "storyline/103.txt"
    },

    // SCENE 5: Neighbor
    {
        5, "Neighbor", "storyline/5.txt", NULL,
        {
            CHOICE("Schedule a coffee",
                6, 1, 1, 1, 0, 0, 0, NO_ITEM),
            CHOICE("Say yes but never follow up",
                7, -1, -1, 0, 0, 0, 0, NO_ITEM)
        },
        2,
        "storyline/105.txt"
    },

    // SCENE 6: Coffee with neighbor
    {
        6, "Coffee with neighbor", "storyline/6.txt", NULL,
        {
            CHOICE("Next day",
                7, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1,
        "storyline/106.txt"
    },

    // SCENE 7: Social media
    // Choices that lead to the first meeting use DEST_MEETING1: it picks 10 (prepared) or 11 (unprepared) depending on whether you read the article.
    {
        7, "Social media", "storyline/7.txt", NULL,
        {
            CHOICE("Believe the post and repost it",
                8, 0, -1, -1, 0, 0, 0, MISINFORMATION_LESSON),
            CHOICE("Investigate and fact-check",
                9, 0, 0, 1, 0, 0, 0, ENVIRONMENTAL_REPORT),
            CHOICE("Ignore the post",
                DEST_MEETING1, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        3
    },

    // SCENE 8: Reposted
    {
        8, "Reposted", "storyline/8.txt", NULL,
        {
            CHOICE("Public meeting",
                DEST_MEETING1, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 9: Fact-checked
    {
        9, "Fact-checked", "storyline/9.txt", NULL,
        {
            CHOICE("Public meeting",
                DEST_MEETING1, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 10: Meeting #1 (prepared)
    {
        10, "Meeting #1 (prepared)", "storyline/10.txt", NULL,
        {
            CHOICE("Continue",
                12, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 11: Meeting #1 (unprepared)
    {
        11, "Meeting #1 (unprepared)", "storyline/11.txt", NULL,
        {
            CHOICE("Continue",
                12, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 12: Public Meeting #1
    {
        12, "Public Meeting #1", "storyline/12.txt", NULL,
        {
            CHOICE("Bring up concerns and ask questions",
                13, 0, 1, 1, 0, 0, 0, NO_ITEM),
            CHOICE("Disagree using factual evidence",
                14, 0, 2, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Disagree using cherry-picked evidence",
                15, 0, -1, -1, 0, 0, 0, NO_ITEM),
            CHOICE("Speak up anyway",
                16, 0, -1, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Stay silent",
                17, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        5,
        "storyline/112.txt"
    },

    // SCENE 13: Concerns (after meeting #1)
    // After the meeting menu
    {
        13, "Concerns (after meeting #1)", "storyline/13.txt", NULL,
        {
            CHOICE("Talk to neighbors",
                18, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Talk to the environmental scientist",
                19, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Go home",
                20, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        3
    },

    // SCENE 14: Factual evidence (after meeting #1)
    {
        14, "Factual evidence (after meeting #1)", "storyline/14.txt", NULL,
        {
            CHOICE("Talk to neighbors",
                18, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Talk to the environmental scientist",
                19, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Go home",
                20, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        3
    },

    // SCENE 15: Cherry-picked evidence (after meeting #1)
    {
        15, "Cherry-picked evidence (after meeting #1)", "storyline/15.txt", NULL,
        {
            CHOICE("Talk to neighbors",
                18, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Talk to the environmental scientist",
                19, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Go home",
                20, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        3
    },

    // SCENE 16: Misstated a fact (after meeting #1)
    {
        16, "Misstated a fact (after meeting #1)", "storyline/16.txt", NULL,
        {
            CHOICE("Talk to neighbors",
                18, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Talk to the environmental scientist",
                19, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Go home",
                20, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        3
    },

    // SCENE 17: Stayed silent (after meeting #1)
    {
        17, "Stayed silent (after meeting #1)", "storyline/17.txt", NULL,
        {
            CHOICE("Talk to neighbors",
                18, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Talk to the environmental scientist",
                19, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Go home",
                20, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        3
    },

    // SCENE 18: Talked to neighbors
    {
        18, "Talked to neighbors", "storyline/18.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 19: Talked to scientist
    {
        19, "Talked to scientist", "storyline/19.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 20: Went home
    {
        20, "Went home", "storyline/20.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 21: Investigation
    // HUB: each activity once. After INVESTIGATION_LIMIT (3) the hub sends you to AFTER_INVESTIGATION (30).
    // The last three activities only appear for Bayview Hunters Point players.
    {
        21, "Investigation", "storyline/21.txt", NULL,
        {
            INV_CHOICE("Discuss the environmental report with the scientist",
                22, 0, 0, 2, SCIENTIST_NOTES, 1 << 0),
            INV_CHOICE("Organize a neighborhood meeting",
                23, 2, 0, 1, NEIGHBORHOOD_SURVEY, 1 << 1),
            INV_CHOICE("Try to meet the company rep",
                27, 0, 0, 1, COMPANY_BROCHURE, 1 << 2),
            INV_CHOICE_CITY("Hear about the biomonitoring pilot",
                127, 1, 0, 1, BIOMONITORING_PROPOSAL, 1 << 3, CITY_BVHP),
            INV_CHOICE_CITY("Visit the construction site",
                128, 0, 0, 1, NO_ITEM, 1 << 4, CITY_BVHP),
            INV_CHOICE_CITY("Try to meet the council member",
                129, 0, 1, 1, COUNCIL_BRIEFING, 1 << 5, CITY_BVHP),
            CHOICE("Move on to the next meeting",
                30, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        7
    },

    // SCENE 22: Scientist
    {
        22, "Scientist", "storyline/22.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 23: Neighborhood meeting
    {
        23, "Neighborhood meeting", "storyline/23.txt", NULL,
        {
            CHOICE("Oppose",
                24, 1, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Support",
                25, 1, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Gather questions",
                26, 1, 0, 1, 0, 0, 0, NO_ITEM)
        },
        3
    },

    // SCENE 24: Opposed
    {
        24, "Opposed", "storyline/24.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 25: Supported
    {
        25, "Supported", "storyline/25.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 26: Gathered questions
    {
        26, "Gathered questions", "storyline/26.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 27: Company rep
    {
        27, "Company rep", "storyline/27.txt", NULL,
        {
            CHOICE("Press them",
                28, 0, 1, 1, 0, 0, 0, NO_ITEM),
            CHOICE("Accept it",
                29, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        2
    },

    // SCENE 28: Pressed the rep
    {
        28, "Pressed the rep", "storyline/28.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 29: Accepted the pitch
    {
        29, "Accepted the pitch", "storyline/29.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 30: The Whistleblower
    {
        30, "The Whistleblower", "storyline/30.txt", NULL,
        {
            CHOICE("Share the information publicly",
                31, 0, 0, 0, 0, 0, 0, NO_ITEM),
            CHOICE("Bring it to the scientist first",
                32, 0, 1, 1, 0, 0, 0, SCIENTIST_NOTES),
            CHOICE("Ignore the message",
                33, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        3
    },

    // SCENE 31: Shared publicly
    {
        31, "Shared publicly", "storyline/31.txt", NULL,
        {
            CHOICE("Continue",
                34, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 32: Told the scientist
    {
        32, "Told the scientist", "storyline/32.txt", NULL,
        {
            CHOICE("Continue",
                34, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 33: Ignored it
    {
        33, "Ignored it", "storyline/33.txt", NULL,
        {
            CHOICE("Continue",
                34, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 34: Public Meeting #2
    // First three need k >= 6. 'Stay quiet' has no requirement so the menu is never empty.
    {
        34, "Public Meeting #2", "storyline/34.txt", NULL,
        {
            CHOICE("Oppose the project",
                35, 0, 0, 0, 0, 0, 6, NO_ITEM),
            CHOICE("Fight for environmental and economic safeguards",
                36, 0, 0, 0, 0, 0, 6, NO_ITEM),
            CHOICE("Push for a moratorium",
                37, 0, 0, 0, 0, 0, 6, NO_ITEM),
            CHOICE("Stay quiet",
                38, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        4
    },

    // SCENE 35: Opposed
    {
        35, "Opposed", "storyline/35.txt", NULL,
        {
            CHOICE("Continue",
                39, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 36: Safeguards
    {
        36, "Safeguards", "storyline/36.txt", NULL,
        {
            CHOICE("Continue",
                40, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 37: Moratorium push
    {
        37, "Moratorium push", "storyline/37.txt", NULL,
        {
            CHOICE("Continue",
                41, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 38: Could not contribute
    {
        38, "Could not contribute", "storyline/38.txt", NULL,
        {
            CHOICE("Continue",
                42, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 39: Rejected
    {
        39, "Rejected", "storyline/39.txt", NULL,
        {
            CHOICE("Five years later",
                43, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 40: Proceeds with safeguards
    {
        40, "Proceeds with safeguards", "storyline/40.txt", NULL,
        {
            CHOICE("Five years later",
                44, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 41: Moratorium granted
    {
        41, "Moratorium granted", "storyline/41.txt", NULL,
        {
            CHOICE("Five years later",
                43, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 42: Limited oversight
    {
        42, "Limited oversight", "storyline/42.txt", NULL,
        {
            CHOICE("Five years later",
                45, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 127: Biomonitoring pilot
    // BVHP only
    {
        127, "Biomonitoring pilot", "storyline/127.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 128: Construction site
    // BVHP only
    {
        128, "Construction site", "storyline/128.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // SCENE 129: Council member
    // BVHP only
    {
        129, "Council member", "storyline/129.txt", NULL,
        {
            CHOICE("Continue",
                21, 0, 0, 0, 0, 0, 0, NO_ITEM)
        },
        1
    },

    // ENDINGS (no choices: the game stops after showing the text)
    { .id = 43, .title = "Ending: green and active", .text_file = "storyline/43.txt", .choice_count = 0,
      .bvhp_file = "storyline/143.txt" },
    { .id = 44, .title = "Ending: growth with guardrails", .text_file = "storyline/44.txt", .choice_count = 0,
      .bvhp_file = "storyline/144.txt" },
    { .id = 45, .title = "Ending: unchecked", .text_file = "storyline/45.txt", .choice_count = 0,
      .bvhp_file = "storyline/145.txt" }
};

int scene_count = sizeof(scenes) / sizeof(scenes[0]);
