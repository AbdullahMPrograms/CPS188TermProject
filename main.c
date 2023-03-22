#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// max lines greather then that of the file
#define MAX_LINES 512
// line char array must be defined to be of length greater then the longest line
#define MAX_LENGTH 512

void parseFile(FILE *file, int *year, char location[][MAX_LENGTH],
               char ageGroup[][MAX_LENGTH], char sex[][MAX_LENGTH],
               double *value, int *numLines, int *canadaStart, int *canadaEnd,
               int *quebecStart, int *quebecEnd, int *ontarioStart,
               int *ontarioEnd, int *albertaStart, int *albertaEnd,
               int *bcStart, int *bcEnd) {
  char line[MAX_LENGTH];
  int i = 0;

  // skip header line
  fgets(line, sizeof(line), file);

  // parse each line of the file
  while (fgets(line, sizeof(line), file) != NULL && i < MAX_LINES) {
    char *token;
    int fieldcount = 0;

    // split the line into fields using comma as delimiter
    // we are "tokenizing" the long string line
    token = strtok(line, ",");

    while (token != NULL && fieldcount < 14) {
      // extract the relevant fields based on their position
      if (fieldcount == 0) {
        // sscanf = string scanf
        sscanf(token, "\"%d\"", &year[i]);
      } else if (fieldcount == 1) {
        sscanf(token, "\"%[^\"]\"", location[i]);

        // Update start and end index of each location

        // if the string in location[i] is identical to "Canada (excluding
        // territories)" strcmp will return 0 (true)
        if (strcmp(location[i], "Canada (excluding territories)") == 0) {
          // if variable canadaStart = -1 that means that the start index of the
          // canada region has not yet been set
          //  therefore we can set it now
          //  -1 is used as it is not a valid array index and therefore does not
          //  interrupt data in the array when used for computation
          if (*canadaStart == -1) {
            *canadaStart = i;
          }
          *canadaEnd = i;
        } else if (strcmp(location[i], "Quebec") == 0) {
          if (*quebecStart == -1) {
            *quebecStart = i;
          }
          *quebecEnd = i;
        } else if (strcmp(location[i], "Ontario") == 0) {
          if (*ontarioStart == -1) {
            *ontarioStart = i;
          }
          *ontarioEnd = i;
        } else if (strcmp(location[i], "Alberta") == 0) {
          if (*albertaStart == -1) {
            *albertaStart = i;
          }
          *albertaEnd = i;
        } else if (strcmp(location[i], "British Columbia") == 0) {
          if (*bcStart == -1) {
            *bcStart = i;
          }
          *bcEnd = i;
        }
      } else if (fieldcount == 3) {
        sscanf(token, "\"%[^\"]\"", ageGroup[i]);
      } else if (fieldcount == 4) {
        sscanf(token, "\"%[^\"]\"", sex[i]);
      } else if (fieldcount == 13) {
        sscanf(token, "\"%lf\"", &value[i]);
      }

      token = strtok(NULL, ",");
      fieldcount++;
    }

    i++;
  }

  *numLines = i;
  /* test code (ensure data is properly being read)
    printf("Number of Lines: %d\n", i);
    printf("\nData for Canada:\n");
    printf("Year, Location, Age Group, Sex, Value\n");
    for (int i = *canadaStart; i <= *canadaEnd; i++) {
      printf("%d, %s, %s, %s, %.2lf\n", year[i], location[i], ageGroup[i],
    sex[i], value[i]);
    }
    */
}

int main() {
  FILE *file = fopen("statscan_diabetes.csv", "r");
  if (file == NULL) {
    printf("Could not open file.\n");
    exit(-1);
  }

  int year[MAX_LINES];
  char location[MAX_LINES][MAX_LENGTH];
  char ageGroup[MAX_LINES][MAX_LENGTH];
  char sex[MAX_LINES][MAX_LENGTH];
  double value[MAX_LINES];
  int numLines = 0;
  double sum = 0;

  int canadaStart = -1, quebecStart = -1, ontarioStart = -1, albertaStart = -1,
      bcStart = -1;
  int canadaEnd = -1, quebecEnd = -1, ontarioEnd = -1, albertaEnd = -1,
      bcEnd = -1;

  parseFile(file, year, location, ageGroup, sex, value, &numLines, &canadaStart,
            &canadaEnd, &quebecStart, &quebecEnd, &ontarioStart, &ontarioEnd,
            &albertaStart, &albertaEnd, &bcStart, &bcEnd);

  fclose(file);

  // print location start and end array index  values for testing
  printf("Canada index values: %d to %d\n", canadaStart, canadaEnd);
  printf("Quebec index values: %d to %d\n", quebecStart, quebecEnd);
  printf("Ontario index values: %d to %d\n", ontarioStart, ontarioEnd);
  printf("Alberta index values: %d to %d\n", albertaStart, albertaEnd);
  printf("British Columbia: %d to %d\n", bcStart, bcEnd);

  // test to see if averaging val does work
  double average;
  for (int j = quebecStart; j <= quebecEnd; j++) {
    sum = sum + value[j];
  }
  average = sum / (quebecEnd - quebecStart);
  printf("1a) Average for Quebec: %.2lf\n", average);

  return 0;
}