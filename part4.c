#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 256 // Maximum string length this program can handle
#define MAX_NB_POKEMON 649 // Maximum number of Pokemons

/*
 * id: ID for each pokemon (not unique)
 * name: Name of each pokemon
 * type1: Each pokemon has a type, this determines weakness/resistance
 * type 2: Some pokemon are dual type and have 2
 * stat_total: sum of all stats, a general guide to how strong a pokemon is
 * hp: hit points, or health, defines how much damage a pokemon can withstand
 * attack: the base modifier for normal attacks (eg. Scratch, Punch)
 * defense: the base damage resistance against normal attacks
 * sp_atk: special attack, the base modifier for special attacks (e.g. Blizzard)
 * sp_def: the base damage resistance against special attacks
 * speed: determines which pokemon attacks first each round
 * gen: Which generation does the pokemon belong to (1 is oldest, 5 newest)
 * legendary: whether or not pokemon is a legendary pokemon (e.g. Moltres, Articuno)
 * desc: A short text description of the pokemon taken from Pokedex
*/
struct Pokemon {
	int id, stat_total, hp, gen, legendary;
	int attack, defense, sp_atk, sp_def, speed;
	char name[MAX_BUFFER], type1[MAX_BUFFER], type2[MAX_BUFFER], desc[MAX_BUFFER];
};

// The CSV parser
int
next_field( FILE *f, char *buf, int max ) {
	int i=0, end=0, quoted=0;
	
	for(;;) {
		// fetch the next character from file		
		buf[i] = fgetc(f);
		// if we encounter quotes then flip our state and immediately fetch next char
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		// end of field on comma if we're not inside quotes
		if(buf[i]==',' && !quoted) { break; }
		// end record on newline or end of file
		if(feof(f) || buf[i]=='\n') { end=1; break; } 
		// truncate fields that would overflow the buffer
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
	return end; // flag stating whether or not this is end of the line
}

// Stuff to make life a bit neater in main
void
fetch_pokemon (  FILE *csv, struct Pokemon *p) {
	char buf[MAX_BUFFER];

	next_field( csv, buf, MAX_BUFFER );      // load id into buffer as string
	p->id = atoi(buf);                       // then parse to integer

	next_field( csv, p->name, MAX_BUFFER );  // name and type are just strings so read
	next_field( csv, p->type1, MAX_BUFFER ); // those directly into the struct
	next_field( csv, p->type2, MAX_BUFFER );

	// Load all the pokemon's stats from the file using buffer as intermediary
	next_field( csv, buf, MAX_BUFFER );
	p->stat_total = atoi(buf);          // atoi stands for ASCII to Integer
	next_field( csv, buf, MAX_BUFFER ); // It converts strings to numbers
	p->hp = atoi(buf);                  // It is not a totally safe function to use.
	next_field( csv, buf, MAX_BUFFER ); // Read the documentation and be safe!
	p->attack = atoi(buf);
	next_field( csv, buf, MAX_BUFFER );
	p->defense = atoi(buf);
	next_field( csv, buf, MAX_BUFFER );
	p->sp_atk = atoi(buf);
	next_field( csv, buf, MAX_BUFFER );
	p->sp_def = atoi(buf);
	next_field( csv, buf, MAX_BUFFER );
	p->speed = atoi(buf);
	next_field( csv, buf, MAX_BUFFER );
	p->gen = atoi(buf);

	// do string compare to get legendary as bool
	next_field( csv, buf, MAX_BUFFER ); 
	p->legendary = (strcmp( buf, "False" )!=0); 

	next_field( csv, p->desc, MAX_BUFFER );
}

void
print_pokemon( struct Pokemon *p ) {
	printf("  %s %s%c%s\n", 
			p->name, p->type1, ((p->type2[0])? '/':' '), p->type2 );
	printf(" --------------------------------------------------------------\n");
	printf("| HP: %4d | ATK: %3d | DEF %3d | SPE %3dd | SAT %3d | SDE %3d |\n",
			p->hp, p->attack, p->defense, p->speed, p->sp_atk, p->sp_def );
	printf(" --------------------------------------------------------------\n");
	printf("%s\n", p->desc);
}

int
main ( int argc, char *argv[] ) {
	FILE *f;
	struct Pokemon pArray[MAX_NB_POKEMON];		
	struct Pokemon p;

	// Users must pass the name of the input file through the command line. Make sure
	// that we got an input file. If not, print a message telling the user how to use
	// the program and quit
	if( argc < 2 ) { 
		printf("usage: csv FILE\n"); 
		return EXIT_FAILURE; 
	}

	// Try to open the input file. If there is a problem, report failure and quit
	f = fopen(argv[1], "r");
	if(!f) { 
		printf("unable to open %s\n", argv[1]); 
		return EXIT_FAILURE; 
	}
	
	fetch_pokemon( f, &p ); // discard the header data in the first line

	// Now read and print pokemon until the end of the file
	int nbPokemons = 0;
	while(!feof(f)) {
		
		fetch_pokemon( f, &pArray[nbPokemons] );
		print_pokemon( &pArray[nbPokemons] );
		printf("\n");
		nbPokemons++;
	}

	float average = 0;
	// now you can do some calculations!
	
	for(int j=0;j<nbPokemons;j++){
		average += (float)pArray[j].attack;
	}

	average = average/(float)nbPokemons;

	printf("average attack = %f \n", average);

	// Always remember to close the file
	fclose(f);
	return EXIT_SUCCESS;
}