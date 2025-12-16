#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX]; // true mean edge from i to j false otherwise

// Each pair has a winner, loser
typedef struct
{
    int winner; // save the index of the winner
    int loser; // save the index of the loser
} pair;

// Array of candidates every index associated with name of a candidate
string candidates[MAX];

// this array of pairs
pair pairs[MAX * (MAX - 1) / 2];

// record the number of pairs
int pair_count;

// record the number of candidates in our program
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1]; // skip the name
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            preferences[i][j] = 0;
        }
    }

    // set the number of pairs to zero
    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        // define array to save the preference of the voter
        // ranks is an array of number of the candidates
        // rank 0 is the most prefered we will set the index of the candidate
        int ranks[candidate_count];

        // Query for each rank
        for (int rank = 0; rank < candidate_count; rank++)
        {
            string name = get_string("Rank %i: ", rank + 1);

            if (!vote(rank, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        // after each voter we record prefernces.
        record_preferences(ranks);

        printf("\n");
    }
    // to here i am ok

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // search for candidate if it found record it's index at ranks
    for (int candidate_index = 0; candidate_index < candidate_count; candidate_index++)
    {
        if (strcmp(name, candidates[candidate_index]) == 0)
        {
            ranks[rank] = candidate_index; // at this rank set index of candidate
            return true;    // return true
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    bool strength;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (i != j && preferences[i][j] != preferences[j][i])
            {

                strength = (preferences[i][j] > preferences[j][i]) ? true : false;

                if (strength)
                {
                    pairs[pair_count].winner = i;
                    pairs[pair_count].loser = j;
                }
                else
                {
                    pairs[pair_count].winner = j;
                    pairs[pair_count].loser = i;
                }

                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int biggest_pair_index, strength_i, strength_j;
    for (int i = 0; i < pair_count - 1; i++)
    {
        biggest_pair_index = i;

        for (int j = i + 1; j < pair_count; j++)
        {
            strength_i = preferences[pairs[biggest_pair_index].winner][pairs[biggest_pair_index].loser];
            strength_j = preferences[pairs[j].winner][pairs[j].loser];

            if (strength_j > strength_i)
            {
                biggest_pair_index = j;
            }
        }

        if (biggest_pair_index != i)
        {
            pair temp = pairs[i];
            pairs[i] = pairs[biggest_pair_index];
            pairs[biggest_pair_index] = temp;
        }
    }
}

bool check_cycle(int winner, int loser)
{
    // base case if the loser is the winner.
    if (loser == winner)
    {
        return true;
    }

    for (int i = 0; i < candidate_count; i++)
    {
        // if any pointer point from the user to any one start from 0 to n - 1
        if (locked[loser][i] == true)
        {
           if (check_cycle(winner, i) == true)
           {
                return true;
           }
        }
    }

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (check_cycle(pairs[i].winner, pairs[i].loser) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }

    return;
}


// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_winner = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                is_winner = false;
                break;
            }
        }

        if (is_winner)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}
