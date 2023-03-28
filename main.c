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
  /*
  printf("\nData for Canada:\n");
  printf("Year, Location, Age Group, Sex, Value\n");
  for (int i = *canadaStart; i <= *canadaEnd; i++) {
    printf("%d, %s, %s, %s, %.2lf\n", year[i], location[i], ageGroup[i], sex[i],
           value[i]);
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

  int canadaStart = -1, quebecStart = -1, ontarioStart = -1, albertaStart = -1,
      bcStart = -1;
  int canadaEnd = -1, quebecEnd = -1, ontarioEnd = -1, albertaEnd = -1,
      bcEnd = -1;

  parseFile(file, year, location, ageGroup, sex, value, &numLines, &canadaStart,
            &canadaEnd, &quebecStart, &quebecEnd, &ontarioStart, &ontarioEnd,
            &albertaStart, &albertaEnd, &bcStart, &bcEnd);

  fclose(file);

  // < ---------------- Q1 ---------------->
  // < ---------------- a) ---------------->
  int count = 0;
  double sum = 0;
  // Quebec average is an array of size 10 to accomodate the yearly averages and
  // the age groups
  // index 0 will be the average excluding years
  // 1-7 will be the yearly averages 2015-2021
  // 8-10 will the be the age groups (35-49, 50-64, 65+)
  double quebecAverage[10];
  sum = 0;
  for (int j = quebecStart; j <= quebecEnd; j++) {
    if (value[j] != 0) {
      sum += value[j];
      count++;
    }
  }
  quebecAverage[0] = sum / count;
  printf("1a) Average for Quebec: %.2lf%\n", quebecAverage[0]);

  double ontarioAverage[10];
  count = 0;
  sum = 0;
  for (int j = ontarioStart; j <= ontarioEnd; j++) {
    if (value[j] != 0) {
      sum += value[j];
      count++;
    }
  }
  ontarioAverage[0] = sum / count;
  printf("1a) Average for Ontario: %.2lf%\n", ontarioAverage[0]);

  count = 0;
  sum = 0;
  double bcAverage[10];
  for (int j = bcStart; j <= bcEnd; j++) {
    if (value[j] != 0) {
      sum += value[j];
      count++;
    }
  }
  bcAverage[0] = sum / count;
  printf("1a) Average for British Columbia: %.2lf%\n", bcAverage[0]);

  count = 0;
  sum = 0;
  double albertaAverage[10];
  for (int j = albertaStart; j <= albertaEnd; j++) {
    if (value[j] != 0) {
      sum += value[j];
      count++;
    }
  }
  albertaAverage[0] = sum / count;
  printf("1a) Average for Alberta: %.2lf%\n", albertaAverage[0]);

  // < ---------------- b) ---------------->
  printf("\n");
  count = 0;
  sum = 0;
  double canadaAverage[10];
  for (int j = canadaStart; j <= canadaEnd; j++) {
    if (value[j] != 0) {
      sum = sum + value[j];
      count++;
    }
  }
  canadaAverage[0] = sum / count;
  printf("1b) Average for Canada: %.2lf%\n", canadaAverage[0]);

  // < ---------------- c) ---------------->
  // 7 years in file (2015-2021)
  // j increments by 7 to go from one 2015 to another 2015
  // i will increment the year 2015 by an amount to go to the next year
  // (2015 --> 2016 --> 2017 etc...)

  // CAN JUST MAKE IF YEAR = 2015 THEN DO BLAH BLAH
  // NO NEED FOR THIS BUT IDK MAN

  printf("\n");
  for (int i = 1; i <= 7; i++) {
    count = 0;
    sum = 0;
    for (int j = canadaStart + i - 1; j <= canadaEnd; j += 7) {
      if (value[j] != 0) {
        sum = sum + value[j];
        count++;
        canadaAverage[i] = sum / count;
      }
    }
    count = 0;
    sum = 0;
    for (int j = ontarioStart + i - 1; j <= ontarioEnd; j += 7) {
      if (value[j] != 0) {
        sum = sum + value[j];
        count++;
        ontarioAverage[i] = sum / count;
      }
    }
    count = 0;
    sum = 0;
    for (int j = quebecStart + i - 1; j <= quebecEnd; j += 7) {
      if (value[j] != 0) {
        sum = sum + value[j];
        count++;
        quebecAverage[i] = sum / count;
      }
    }
    count = 0;
    sum = 0;
    for (int j = bcStart + i - 1; j <= bcEnd; j += 7) {
      if (value[j] != 0) {
        sum = sum + value[j];
        count++;
        bcAverage[i] = sum / count;
      }
    }
    count = 0;
    sum = 0;
    for (int j = albertaStart + i - 1; j <= albertaEnd; j += 7) {
      if (value[j] != 0) {
        sum = sum + value[j];
        count++;
        albertaAverage[i] = sum / count;
      }
    }

    printf("1c) Average for year %d:\nCanada: %.2lf%\nOntario: %.2lf%\nQuebec: "
           "%.2lf%\nBC: %.2lf%\nAlberta: %.2lf%\n\n",
           (2015 + i - 1), canadaAverage[i], ontarioAverage[i],
           quebecAverage[i], bcAverage[i], albertaAverage[i]);
  }
  // < ---------------- d) ---------------->
  count = 0;
  sum = 0;
  for (int j = ontarioStart; j <= ontarioEnd; j++) {
    if (strcmp(ageGroup[j], "35 to 49 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  ontarioAverage[8] = sum / count;
  printf("1d) Average for Ontario (35-49 years): %.2lf%\n", ontarioAverage[8]);

  count = 0;
  sum = 0;
  for (int j = ontarioStart; j <= ontarioEnd; j++) {
    if (strcmp(ageGroup[j], "50 to 64 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  ontarioAverage[9] = sum / count;
  printf("1d) Average for Ontario (50-64 years): %.2lf%\n", ontarioAverage[9]);

  count = 0;
  sum = 0;
  for (int j = ontarioStart; j <= ontarioEnd; j++) {
    if (strcmp(ageGroup[j], "65 years and over") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  ontarioAverage[10] = sum / count;
  printf("1d) Average for Ontario (65+ years): %.2lf%\n", ontarioAverage[10]);

  printf("\n");

  count = 0;
  sum = 0;
  for (int j = canadaStart; j <= canadaEnd; j++) {
    if (strcmp(ageGroup[j], "35 to 49 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  canadaAverage[8] = sum / count;
  printf("1d) Average for Canada (35-49 years): %.2lf%\n", canadaAverage[8]);

  count = 0;
  sum = 0;
  for (int j = canadaStart; j <= canadaEnd; j++) {
    if (strcmp(ageGroup[j], "50 to 64 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  canadaAverage[9] = sum / count;
  printf("1d) Average for Canada (50-64 years): %.2lf%\n", canadaAverage[9]);

  count = 0;
  sum = 0;
  for (int j = canadaStart; j <= canadaEnd; j++) {
    if (strcmp(ageGroup[j], "65 years and over") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  canadaAverage[10] = sum / count;
  printf("1d) Average for Canada (65+ years): %.2lf%\n", canadaAverage[10]);

  printf("\n");

  count = 0;
  sum = 0;
  for (int j = quebecStart; j <= quebecEnd; j++) {
    if (strcmp(ageGroup[j], "35 to 49 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  quebecAverage[8] = sum / count;
  printf("1d) Average for Quebec (35-49 years): %.2lf%\n", quebecAverage[8]);

  count = 0;
  sum = 0;
  for (int j = quebecStart; j <= quebecEnd; j++) {
    if (strcmp(ageGroup[j], "50 to 64 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  quebecAverage[9] = sum / count;
  printf("1d) Average for Quebec (50-64 years): %.2lf%\n", quebecAverage[9]);

  count = 0;
  sum = 0;
  for (int j = quebecStart; j <= quebecEnd; j++) {
    if (strcmp(ageGroup[j], "65 years and over") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  quebecAverage[10] = sum / count;
  printf("1d) Average for Quebec (65+ years): %.2lf%\n", quebecAverage[10]);

  printf("\n");

  count = 0;
  sum = 0;
  for (int j = albertaStart; j <= albertaEnd; j++) {
    if (strcmp(ageGroup[j], "35 to 49 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  albertaAverage[8] = sum / count;
  printf("1d) Average for Alberta (35-49 years): %.2lf%\n", albertaAverage[8]);

  count = 0;
  sum = 0;
  for (int j = albertaStart; j <= albertaEnd; j++) {
    if (strcmp(ageGroup[j], "50 to 64 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  albertaAverage[9] = sum / count;
  printf("1d) Average for Alberta (50-64 years): %.2lf%\n", albertaAverage[9]);

  count = 0;
  sum = 0;
  for (int j = albertaStart; j <= albertaEnd; j++) {
    if (strcmp(ageGroup[j], "65 years and over") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  albertaAverage[10] = sum / count;
  printf("1d) Average for Alberta (65+ years): %.2lf%\n", albertaAverage[10]);

  printf("\n");

  count = 0;
  sum = 0;
  for (int j = bcStart; j <= bcEnd; j++) {
    if (strcmp(ageGroup[j], "35 to 49 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  bcAverage[8] = sum / count;
  printf("1d) Average for British Columbia (35-49 years): %.2lf%\n",
         bcAverage[8]);

  count = 0;
  sum = 0;
  for (int j = bcStart; j <= bcEnd; j++) {
    if (strcmp(ageGroup[j], "50 to 64 years") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  bcAverage[9] = sum / count;
  printf("1d) Average for British Columbia (50-64 years): %.2lf%\n",
         bcAverage[9]);

  count = 0;
  sum = 0;
  for (int j = bcStart; j <= bcEnd; j++) {
    if (strcmp(ageGroup[j], "65 years and over") == 0) {
      if (value[j] != 0) {
        sum += value[j];
        count++;
      }
    }
  }
  bcAverage[10] = sum / count;
  printf("1d) Average for British Columbia (65+ years): %.2lf%\n",
         bcAverage[10]);

  // < ---------------- Q2 ---------------->
  printf("\n");
  // initialize min and max variables
  double min = quebecAverage[0];
  double max = quebecAverage[0];
  char *lowestLocation = "Quebec";
  char *highestLocation = "Quebec";

  // compare with the other variables
  if (bcAverage[0] < min) {
    min = bcAverage[0];
    lowestLocation = "British Columbia";
  }
  if (albertaAverage[0] < min) {
    min = albertaAverage[0];
    lowestLocation = "Alberta";
  }
  if (ontarioAverage[0] < min) {
    min = ontarioAverage[0];
    lowestLocation = "Ontario";
  }

  if (bcAverage[0] > max) {
    max = bcAverage[0];
    highestLocation = "British Columbia";
  }
  if (albertaAverage[0] > max) {
    max = albertaAverage[0];
    highestLocation = "Alberta";
  }
  if (ontarioAverage[0] > max) {
    max = ontarioAverage[0];
    highestLocation = "Ontario";
  }

  printf("2) The province with the lowest average is: %.2lf in %s\n", min,
         lowestLocation);
  printf("2) The province with the greatest average is: %.2lf in %s\n", max,
         highestLocation);

  // < ---------------- Q3 ---------------->
  printf("\n");
  printf("3) Provinces with averages above the national average:\n");
  if (bcAverage[0] > canadaAverage[0]) {
    printf("British Columbia\n");
  }
  if (ontarioAverage[0] > canadaAverage[0]) {
    printf("Ontario\n");
  }
  if (albertaAverage[0] > canadaAverage[0]) {
    printf("Alberta\n");
  }
  if (quebecAverage[0] > canadaAverage[0]) {
    printf("Quebec\n");
  }
  printf("\n");
  printf("3) Provinces with averages below the national average:\n");
  if (bcAverage[0] < canadaAverage[0]) {
    printf("British Columbia\n");
  }
  if (ontarioAverage[0] < canadaAverage[0]) {
    printf("Ontario\n");
  }
  if (albertaAverage[0] < canadaAverage[0]) {
    printf("Alberta\n");
  }
  if (quebecAverage[0] < canadaAverage[0]) {
    printf("Quebec\n");
  }
  // < ---------------- Q4 ---------------->
  printf("\n");
  // Find the highest value and its location
  double highestValue = quebecAverage[1];
  int highestYear = 2015;

  for (int i = 1; i <= 7; i++) {
    if (quebecAverage[i] > highestValue) {
      highestValue = quebecAverage[i];
      highestYear = 2014 + i;
      highestLocation = "Quebec";
    }

    if (bcAverage[i] > highestValue) {
      highestValue = bcAverage[i];
      highestYear = 2014 + i;
      highestLocation = "British Columbia";
    }

    if (albertaAverage[i] > highestValue) {
      highestValue = albertaAverage[i];
      highestYear = 2014 + i;
      highestLocation = "Alberta";
    }

    if (ontarioAverage[i] > highestValue) {
      highestValue = ontarioAverage[i];
      highestYear = 2014 + i;
      highestLocation = "Ontario";
    }
  }

  // Find the lowest value and its location
  double lowestValue = quebecAverage[1];
  int lowestYear = 2015;

  for (int i = 1; i <= 7; i++) {
    if (quebecAverage[i] < lowestValue) {
      lowestValue = quebecAverage[i];
      lowestYear = 2014 + i;
      lowestLocation = "Quebec";
    }

    if (bcAverage[i] < lowestValue) {
      lowestValue = bcAverage[i];
      lowestYear = 2014 + i;
      lowestLocation = "British Columbia";
    }

    if (albertaAverage[i] < lowestValue) {
      lowestValue = albertaAverage[i];
      lowestYear = 2014 + i;
      lowestLocation = "Alberta";
    }

    if (ontarioAverage[i] < lowestValue) {
      lowestValue = ontarioAverage[i];
      lowestYear = 2014 + i;
      lowestLocation = "Ontario";
    }
  }

  // Print the results
  printf("4) The Province and year with the highest value is: %.2f in %s, %d\n",
         highestValue, highestLocation, highestYear);
  printf("4) The Province and year with the lowest value is: %.2f in %s, %d\n",
         lowestValue, lowestLocation, lowestYear);

  return 0;
}