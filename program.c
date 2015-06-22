/*
 * Copyright (C) 2002 John Todd Larason <jtl@molehill.org>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 */

#include <string.h>
#include "rtv.h"
#include "dump.h"
#include "program.h"

struct mapping program_flags_mapping[] = {
    { 1 << 0, "Closed Captioned"},
    { 1 << 1, "Stereo"},
    { 1 << 2, "Repeat"},
    { 1 << 3, "Second Audio Program"},
    { 1 << 4, "Letterboxed"},
    { 1 << 5, "Movie"},
    { 1 << 6, "Parts"},
    { 1 << 7, "Pay-Per-View"},
    { 1 << 11, "TV-14"},        /* only valid if Movie flag clear */
    { 1 << 12, "TV-G"},         /* only valid if Movie flag clear */
    { 1 << 13, "TV-MA"},        /* only valid if Movie flag clear */ 
    { 1 << 14, "TV-PG"},        /* only valid if Movie flag clear */
    { 1 << 15, "TV-Y"},         /* only valid if Movie flag clear */
    { 1 << 16, "TV-Y7"},        /* only valid if Movie flag clear */
    { 1 << 17, "TV:S"},
    { 1 << 18, "TV:V"},
    { 1 << 19, "TV:L"},
    { 1 << 20, "TV:D"},
    { 1 << 21, "TV:FV"},
    { 1 << 22, "Movie:AC"},
    { 1 << 23, "Movie:BN"},
    { 1 << 24, "Movie:GL"},
    { 1 << 25, "Movie:GV"},
    { 1 << 26, "Movie:AL"},
    { 1 << 27, "Movie:MV"},
    { 1 << 28, "Movie:N"},
    { 1 << 29, "Movie:RP"},
    { 1 << 30, "Movie:SC"},
    { 1 << 31, "Movie:V"},
    {-1, NULL}
};

struct mapping mpaa_mapping[] = {
    { 1 << 0, "AO" },
    { 1 << 1, "G" },
    { 1 << 2, "NC17" },
    { 1 << 3, "NR" },
    { 1 << 4, "PG" },
    { 1 << 5, "PG13" },
    { 1 << 6, "R" },
    { -1, NULL }
};

struct mapping genre_mapping[] = {
    {1, "Action" },
    {2, "AdultsOnly" },
    {3, "Adventure" },
    {4, "Animals" },
    {5, "Animated" },
    {6, "AnimatedMusical" },
    {7, "Anthology" },
    {8, "Art" },
    {9, "Auto" },
    {10, "Awards" },
    {11, "Ballet" },
    {12, "Baseball" },
    {13, "Basketball" },
    {14, "Beauty" },
    {15, "Bicycle" },
    {16, "Billiards" },
    {17, "Biography" },
    {18, "Boat" },
    {19, "Bodybuilding" },
    {20, "Bowling" },
    {21, "Boxing" },
    {22, "BusFinancial" },
    {23, "BusFinancialSpecial" },
    {24, "BusFinancialTalk" },
    {25, "Business" },
    {26, "Children" },
    {27, "ChildrenSpecial" },
    {28, "ChildrenTalk" },
    {29, "ChildrensMusic" },
    {30, "Classic" },
    {31, "Collectibles" },
    {32, "Comedy" },
    {33, "ComedyDrama" },
    {34, "Computers" },
    {35, "Cooking" },
    {36, "Crime" },
    {37, "CrimeDrama" },
    {38, "Curling" },
    {39, "Dance" },
    {40, "Diving" },
    {41, "Docudrama" },
    {42, "Documentary" },
    {43, "Drama" },
    {44, "Educational" },
    {45, "Electronics" },
    {46, "Event" },
    {47, "Exercise" },
    {48, "Family" },
    {49, "Fantasy" },
    {50, "Fashion" },
    {51, "Fiction" },
    {52, "Fishing" },
    {53, "Football" },
    {54, "French" },
    {55, "Fundraiser" },
    {56, "Game" },
    {57, "Golf" },
    {58, "Gymnastics" },
    {59, "Health" },
    {60, "Historical" },
    {61, "HistoricalDrama" },
    {62, "Hockey" },
    {63, "Holiday" },
    {64, "HolidayChildren" },
    {65, "HolidayChildrensSpecial" },
    {66, "HolidayMusic" },
    {67, "HolidayMusicSpecial" },
    {68, "HolidaySpecial" },
    {69, "Horror" },
    {70, "Horse" },
    {71, "HouseGarden" },
    {72, "Housewares" },
    {73, "HowTo" },
    {74, "International" },
    {75, "Interview" },
    {76, "Jewelry" },
    {77, "Lacrosse" },
    {78, "Magazine" },
    {79, "MartialArts" },
    {80, "Medical" },
    {81, "Miniseries" },
    {82, "Motor" },
    {83, "Motorcycle" },
    {84, "Music" },
    {85, "MusicSpecial" },
    {86, "MusicTalk" },
    {87, "Musical" },
    {88, "MusicalComedy" },
    {89, "MusicalRomance" },
    {90, "Mystery" },
    {91, "Nature" },
    {92, "News" },
    {93, "NonEvent" },
    {94, "Olympics" },
    {95, "Opera" },
    {96, "Outdoors" },
    {97, "ParentalAdvisory" },
    {98, "Play" },
    {99, "PublicAffairs" },
    {100, "Racing" },
    {101, "Racquet" },
    {102, "Reality" },
    {103, "Religious" },
    {104, "Rodeo" },
    {105, "Romance" },
    {106, "RomanceComedy" },
    {107, "Rugby" },
    {108, "Running" },
    {109, "Satire" },
    {110, "Science" },
    {111, "ScienceFiction" },
    {112, "SelfHelp" },
    {113, "Shopping" },
    {114, "Situation" },
    {115, "Skating" },
    {116, "Skiing" },
    {117, "SledDogs" },
    {118, "Snow" },
    {119, "Soap" },
    {120, "SoapOpera" },
    {121, "SoapSpecial" },
    {122, "SoapTalk" },
    {123, "Soccer" },
    {124, "Softball" },
    {125, "Spanish" },
    {126, "Special" },
    {127, "Sports" },
    {128, "SportsEvent" },
    {129, "SportsTalk" },
    {130, "SportsNonEvent" },
    {131, "Suspense" },
    {132, "SuspenseComedy" },
    {133, "Swimming" },
    {134, "Talk" },
    {135, "Tennis" },
    {136, "Thriller" },
    {137, "TrackField" },
    {138, "Travel" },
    {139, "Variety" },
    {140, "Volleyball" },
    {141, "War" },
    {142, "Water" },
    {143, "Weather" },
    {144, "Western" },
    {145, "WesternComedy" },
    {146, "Wrestling" },
    {147, "FrenchBiographie" },
    {148, "FrenchComedie" },
    {149, "FrenchComedieDramatique" },
    {150, "FrenchComedieMusicale" },
    {151, "FrenchComedieSentimentale" },
    {152, "FrenchComedieWestern" },
    {153, "FrenchDessinsAnimes" },
    {154, "FrenchDocudrame" },
    {155, "FrenchDocumentaire" },
    {156, "FrenchDrame" },
    {157, "FrenchDrameDAction" },
    {158, "FrenchDrameDAventure" },
    {159, "FrenchDrameDHorreur" },
    {160, "FrenchDrameDeGuerre" },
    {161, "FrenchDramePolicier" },
    {162, "FrenchDrameHistorique" },
    {163, "FrenchDrameSentimental" },
    {164, "FrenchEnfants" },
    {165, "FrenchFantaisie" },
    {166, "FrenchMiniSerie" },
    {167, "FrenchMusicale" },
    {168, "FrenchMusicaleAnimee" },
    {169, "FrenchMusicaleSentimentale" },
    {170, "FrenchMystere" },
    {171, "FrenchPieceFilmee" },
    {172, "FrenchPourAdults" },
    {173, "FrenchScienceFiction" },
    {174, "FrenchSuspense" },
    {175, "FrenchSuspenseComedie" },
    {176, "FrenchWestern" },
    {-1, NULL}
};

int parse_program(unsigned char **pp, struct program * prog)
{
    unsigned char * p = *pp;
    
    memset(prog, 0, sizeof prog);
    
    prog->struct_size      = rtv_to_u32(&p);
    prog->unknown1         = rtv_to_u32(&p);
    prog->isvalid          = rtv_to_u32(&p);
    prog->tuning           = rtv_to_u32(&p);
    parse_fixed_program_record(&p, &prog->fixed_program_record, 0);

    if (prog->struct_size) {
        expect(prog->struct_size == 272);
        expect(prog->unknown1 == 1);
        expect(prog->isvalid == 1);
    }

    *pp = p;
    
    return 0;
}

void dump_program(struct program * p)
{
    if (!p->isvalid)
        return;

    dump_group_start    ("Program");
    dump_u32            ("Struct Size",        p->struct_size);
    dump_u32            ("Unknown1",           p->unknown1);
    dump_u32            ("Is Valid",           p->isvalid);
    dump_u32            ("Tuning",             p->tuning);
    dump_fixed_program_record(                &p->fixed_program_record);
    dump_group_end      ();
}
