# Nibras Faruque G01078180
# Biking Software

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TRIP_COUNT 20000

// define structure of trip
typedef struct Trip_struct{
    char membershipType[12];
    int startStationId, endStationId, bikeId, duration, startHr, startMin;
}Trip;

// define structure of bike
typedef struct Bike_struct{
   char maintenanceFlag;
   int bikeId, endStationId;
   int numTrips, totalDuration;
   float totalMinutes;
}Bike;

// This function is used to display main menu
void displayMainMenu(){
    printf("\nMain Menu");
    printf("\n---------");
    printf("\n1) Read Trip Data");
    printf("\n2) Run Metrics");
    printf("\n3) Print Menu");
    printf("\n4) Flag Bikes for Maintenance");
    printf("\n5) Add Bike to Inventory");
    printf("\n6) Delete Bike from Inventory");
    printf("\n7) Quit");
}

// This function is used to get valid choice from the given interval
int getValidChoice(int minChoice, int maxChoice){
    int choice=-1;
    // loop until valid choice is entered
    while(choice<minChoice||choice>maxChoice){
        // prompt and get choice
        printf("\nPlease enter your choice: ");
        scanf("%d",&choice);

        // if choice is invalid
        if(choice<minChoice||choice>maxChoice){
            // display error message
            printf("Invalid Choice.");
        }
    }
    return choice;
}

// This function is used to get filename from user
char *getFilename(){
    char *filename=(char*)malloc(30*sizeof(char));
    // prompt and get filename
    printf("Enter the filename: ");
    scanf("%s",filename);

    // return filename
    return filename;
}

// This function is used to read trip file and update the tripData
void readTripFile(FILE *inputFile, Trip* tripData, int tripCount){
    // loop for i from 0 to tripCount
    for(int i=0;i<tripCount;i++){
        // read ith trip details
        fscanf(inputFile, "%s %d %d %d %d %d %d\n",
               tripData[i].membershipType,&tripData[i].startStationId,&tripData[i].endStationId,
               &tripData[i].bikeId,&tripData[i].duration, &tripData[i].startHr,&tripData[i].startMin);
    }
}

// This function is used to read trip data from file and return the trip detail
Trip* readTripData(int *tripCount){
    Trip *tripData=NULL;
    // get the filename
    char *filename=getFilename();

    // open the file in read mode
    FILE *inputFile=fopen(filename, "r");

    // if file does not exist
    if(inputFile==NULL){
        // display error message
        printf("Input file %s does not exist.",filename);
    }
    else{
        // read number of trips from file
        fscanf(inputFile, "%d\n", tripCount);

        // create array of structures to store specific number of trips
        tripData = malloc(*tripCount * sizeof(Trip));

        // read trip file and update tripData array
        readTripFile(inputFile,tripData,*tripCount);

        // display success message
        printf("Data loaded successfully!");
    }
    // return trip details
    return tripData;
}

// This function is used to display metrics menu
void displayMetricsMenu(){
    printf("\nMetrics Menu");
    printf("\n------------");
    printf("\n1) Average number of trips per hour");
    printf("\n2) The hour with largest number of trips");
    printf("\n3) A report of the count of trips in each hour");
	printf("\n4) Average duration of the trips");
	printf("\n5) The trip with the longest duration");
	printf("\n6) The 5 busiest starting stations");
    printf("\n7) The 5 busiest ending stations");
    printf("\n8) The percentage of trips in each membership type");
}

// This function is used to get valid choice of Y or N from user
int getYorN(char *prompt){
    char choice;
    int valid=0;
    // loop until valid choice is entered
    while(!valid){
        // prompt and get choice
        printf(prompt);
        scanf(" %c",&choice);

        // if choice is y or n irrespective of case
        if(choice=='y' || choice=='Y' || choice=='n' || choice=='N'){
            // set valid to 1
            valid=1;
        }
        else{
            // display error message
            printf("Please enter Y or N\n");
        }
    }
    // return if choice is y or n
    return choice=='y' || choice=='Y';
}

// This function is used to get number of trips per hour
int *getNoOfTripsPerHour(Trip* tripData, int tripCount){
    // create array of integer to store number of trips for each hour
    int *noOfTripsPerHour=(int *) malloc(24*sizeof(int));

    // initialize the number of trips for each hour to 0
    for(int hour=0;hour<24;hour++){
        noOfTripsPerHour[hour]=0;
    }

    // loop for each trips
    for(int i=0;i<tripCount;i++){
        // increment the noOfTripsPerHour of particular hour by 1
        noOfTripsPerHour[tripData[i].startHr]++;
    }
    // return number of trips per hour
    return noOfTripsPerHour;
}

// This function is used to return average number of trips per hour
float getAvgNoOfTripsPerHour(Trip* tripData, int tripCount){
    // determine number of trips per hour in a day
    int *noOfTripsPerHour=getNoOfTripsPerHour(tripData, tripCount);

    int totalTrips=0;
    // loop for each hour in a day and determine total trips in each hour
    for(int hour=0;hour<24;hour++){
        totalTrips +=noOfTripsPerHour[hour];
    }
    // determine and return average number of trips per hour
    float avgNoOfTripsPerHour=totalTrips/24.0;
    return avgNoOfTripsPerHour;
}

// This function is used to determine the hour with maximum number of trips
int getHourWithMaxTripCount(Trip* tripData, int tripCount){
    // get number of trips in each hour
    int *noOfTripsPerHour=getNoOfTripsPerHour(tripData, tripCount);
    int maxHour=0;
    // loop for each hour and determine hour with maximum number of trips
    for(int hour=1;hour<24;hour++){
        if(noOfTripsPerHour[hour]>noOfTripsPerHour[maxHour]){
            maxHour=hour;
        }
    }
    // return hour with maximum number of trip
    return maxHour;
}

// This function is used determine and return average duration of trips in milliseconds
float getAvgDurationOfTripsInMS(Trip* tripData, int tripCount){
    int totalDuration=0;
    // loop for each trips and determine total duration
    for(int i=0;i<tripCount;i++){
        totalDuration += tripData[i].duration;
    }
    // determine and display average duration of trips in ms
    float avgDurationOfTripsInMS=0;
    if(tripCount>0)
        avgDurationOfTripsInMS=totalDuration/(tripCount*1.0);
    return avgDurationOfTripsInMS;
}

// This function is used to convert milliseconds to minute
float getMinuteFromMS(int milliseconds){
    // return minutes from milliseconds
    return milliseconds/(1000*60*1.0);
}

// This function is used to return index of trip with longest duration trip
int getLongestDurationTrip(Trip* tripData, int tripCount){
    int longDurationTripIndex=-1;
    // if number of trips is greater than 0
    if(tripCount>0){
        longDurationTripIndex=0;
        // loop for each trip and determine index of trip with long duration
        for(int i=1;i<tripCount;i++){
            if(tripData[i].duration>tripData[longDurationTripIndex].duration){
                longDurationTripIndex=i;
            }
        }
    }
    // return longest duration trip index
    return longDurationTripIndex;
}

// This function is used to display the details of trip with particular trip index
void displayTrip(FILE *output,Trip *tripData,int tripCount, int tripIndex){
    // display details of trip with particular trip index
    fprintf(output,"Trip Membership Type: %s\n",tripData[tripIndex].membershipType);
    fflush(output);
    fprintf(output,"Trip Start Station ID: %d\n",tripData[tripIndex].startStationId);
    fflush(output);
    fprintf(output,"Trip End Station ID: %d\n",tripData[tripIndex].endStationId);
    fflush(output);
    fprintf(output,"Trip Bike ID: %d\n",tripData[tripIndex].bikeId);
    fflush(output);
    fprintf(output,"Trip Duration: %d\n",tripData[tripIndex].duration);
    fflush(output);
    fprintf(output,"Trip Start Hour: %d\n",tripData[tripIndex].startHr);
    fflush(output);
    fprintf(output,"Trip Start Minute: %d\n",tripData[tripIndex].startMin);
    fflush(output);
}

// This function is used to swap two integers
void swap(int *a, int *b){
    int temp=*a;
    *a=*b;
    *b=temp;
}

// This function is used to determine top 5 busiest starting station
int *getBusiestStartingStation(Trip* tripData, int tripCount){
    int* busiestStartingStation=(int *)malloc(5*sizeof(int));

    // create array to store list of unique starting station and number of trip starting from station
    int startingStation[tripCount];
    int tripCountOfStation[tripCount];
    int startingStationCount=0;

    // loop for each trips
    for(int i=0;i<tripCount;i++){
        int foundStartingStationIndex=-1;
        // check if starting station id already exist
        for(int j=0;j<startingStationCount;j++){
            if(startingStation[j]==tripData[i].startStationId){
                foundStartingStationIndex=j;
            }
        }
        // if starting station is not found
        if(foundStartingStationIndex==-1){
            // append starting station to array
            startingStation[startingStationCount]=tripData[i].startStationId;
            tripCountOfStation[startingStationCount]=1;

            // increment number of starting station count by 1
            startingStationCount++;
        }
        else{
            // increment number of trips from particular station by 1
            tripCountOfStation[foundStartingStationIndex]+=1;
        }
    }

    // sort starting station array based on number of trips in descending order
    for(int i=0;i<startingStationCount;i++){
        for(int j=i+1;j<startingStationCount;j++){
            if(tripCountOfStation[i]<tripCountOfStation[j]){
                swap(&startingStation[i],&startingStation[j]);
                swap(&tripCountOfStation[i],&tripCountOfStation[j]);
            }
        }
    }

    // determine and top 5 busiest starting station
    for(int i=0;i<5;i++){
        busiestStartingStation[i]=startingStation[i];
    }
    return busiestStartingStation;
}

// This function is used to determine top 5 busiest ending station
int *getBusiestEndingStation(Trip* tripData, int tripCount){
    int* busiestEndingStation=(int *)malloc(5*sizeof(int));

    // create array to store list of unique ending station and number of trip starting from station
    int endingStation[tripCount];
    int tripCountOfStation[tripCount];
    int endingStationCount=0;

    // loop for each trips
    for(int i=0;i<tripCount;i++){
        int foundEndingStationIndex=-1;
        // check if ending station id already exist
        for(int j=0;j<endingStationCount;j++){
            if(endingStation[j]==tripData[i].endStationId){
                foundEndingStationIndex=j;
            }
        }

        // if ending station is not found
        if(foundEndingStationIndex==-1){
            // append ending station to array
            endingStation[endingStationCount]=tripData[i].endStationId;
            tripCountOfStation[endingStationCount]=1;

            // increment number of ending station count by 1
            endingStationCount++;
        }
        else{
            tripCountOfStation[foundEndingStationIndex]+=1;
        }
    }

    // sort ending station array based on number of trips in descending order
    for(int i=0;i<endingStationCount;i++){
        for(int j=i+1;j<endingStationCount;j++){
            if(tripCountOfStation[i]<tripCountOfStation[j]){
                swap(&endingStation[i],&endingStation[j]);
                swap(&tripCountOfStation[i],&tripCountOfStation[j]);
            }
        }
    }

    // determine and top 5 busiest ending station
    for(int i=0;i<5;i++){
        busiestEndingStation[i]=endingStation[i];
    }
    return busiestEndingStation;
}

// This function is used to determine number of trips with casual membership type
float getPercentageOfCasualTrip(Trip* tripData, int tripCount){
    int casualTripCount=0;
    // loop for each trips
    for(int i=0;i<tripCount;i++){
        // if member ship is casual
        if(strcmp(tripData[i].membershipType,"Casual")==0){
            // increment casual trip count by 1
            casualTripCount++;
        }
    }
    float percentageOfCasualTrip=0;
    // if trip count is greater than 0
    if(tripCount>0){
        // determine percentage of casual trip
        percentageOfCasualTrip=(100*casualTripCount)/(1.0*tripCount);
    }
    return percentageOfCasualTrip;
}

// This function is used to display average number of trips per hour
void displayAvgNoOfTripsPerHour(FILE *output,Trip *tripData,int tripCount){
    // determine and display average number of trips per hour
    float avgNoOfTripsPerHour = getAvgNoOfTripsPerHour(tripData,tripCount);
    fprintf(output,"Average number of trips per hour is %.2f",avgNoOfTripsPerHour);
    fflush(output);
}

// This function is used to display hour with largest number of trips
void displayHourWithMaxTripCount(FILE *output,Trip *tripData,int tripCount){
    // determine and display hour with largest number of trips
    int maxHour=getHourWithMaxTripCount(tripData,tripCount);
    fprintf(output,"The hour with largest number of trips is %d",maxHour);
    fflush(output);
}

// This function is used to display number of trips in each hour
void displayNoOfTripsPerHour(FILE *output,Trip *tripData,int tripCount){
    // determine number of trips in each hour
    int *noOfTripsPerHour =getNoOfTripsPerHour(tripData,tripCount);
    // display the hour and number of trips at that hour
    fprintf(output,"%-10s %s\n","Hour","Number of Trips");
    fflush(output);
    for(int hour=0;hour<24;hour++){
        fprintf(output,"%-10d %d\n",hour,noOfTripsPerHour[hour]);
        fflush(output);
    }
}

// This function is used to display average duration of the trips in ms and minutes
void displayAvgDurationOfTrips(FILE *output,Trip *tripData,int tripCount){
    // determine and display average duration of the trips in ms and minutes
    float avgDurationOfTripsInMS = getAvgDurationOfTripsInMS(tripData,tripCount);
    fprintf(output,"Average duration of the trips is %.2f ms or %.2f minutes",avgDurationOfTripsInMS,getMinuteFromMS(avgDurationOfTripsInMS));
    fflush(output);
}

// This function is used to display trip with the longest duration
void displayLongestDurationTrip(FILE *output,Trip *tripData,int tripCount){
    // determine index of trip with longest duration
    int tripIndex = getLongestDurationTrip(tripData,tripCount);

    // if trip index is valid
    if(tripIndex>=0){
        // display trip with longest duration
        fprintf(output,"The trip with the longest duration is \n");
        fflush(output);
        displayTrip(output,tripData,tripCount,tripIndex);
    }
    else{
        // display error message
        fprintf(output,"No trip exist.\n");
        fflush(output);
    }
}

// This function is used to display 5 busiest starting stations
void display5BusiestStartingStation(FILE *output,Trip *tripData,int tripCount){
    // if number of trips is greater than 0
    if(tripCount>0){
        // determine 5 busiest starting stations
        int *busiestStartingStation = getBusiestStartingStation(tripData,tripCount);

        // display 5 busiest starting stations
        fprintf(output,"The 5 busiest starting stations\n");
        fflush(output);
        for(int i=0;i<5;i++){
            fprintf(output,"%d\n",busiestStartingStation[i]);
            fflush(output);
        }
    }
    else{
        // display error message
        fprintf(output,"No trip exist.\n");
        fflush(output);
    }
}

// This function is used to display 5 busiest ending stations
void display5BusiestEndingStation(FILE *output,Trip *tripData,int tripCount){
    // if number of trips is greater than 0
    if(tripCount>0){
        // determine 5 busiest ending stations
        int *busiestEndingStation = getBusiestEndingStation(tripData,tripCount);

        // display 5 busiest ending stations
        fprintf(output,"The 5 busiest ending stations\n");
        fflush(output);
        for(int i=0;i<5;i++){
            fprintf(output,"%d\n",busiestEndingStation[i]);
            fflush(output);
        }
    }
    else{
        // display error message
        fprintf(output,"No trip exist.");
        fflush(output);
    }
}

// This function is used to display percentage of trips for each membership type
void displayPercentageOfMembershipType(FILE *output,Trip *tripData,int tripCount){
    // determine number of percentage of trips for casual membership type
    float casualMembershipPercent=getPercentageOfCasualTrip(tripData,tripCount);

    // determine number of percentage of trips for registered membership type
    float regMembershipPercent=0;
    if(tripCount>0)
        regMembershipPercent=100-casualMembershipPercent;

    // display percentage of trips of registered membership type
    fprintf(output,"Percentage of trips of registered membership type is %.2f%%\n",regMembershipPercent);
    fflush(output);

    // display percentage of trips of casual membership type
    fprintf(output,"Percentage of trips of casual membership type is %.2f%%\n",casualMembershipPercent);
    fflush(output);
}

// This function is used to run metrics
void runMetrics(Trip *tripData,int tripCount){
    // display metrics menu
    displayMetricsMenu();
    // get valid choice
    int choice=getValidChoice(1,8);
    printf("\n");

    // check if output to be written to file
    int doWriteInFile=getYorN("Do require to write the result to an output file? (y or n): ");
    char *filename="";
    FILE *output=stdout;

    // if to write to file
    if(doWriteInFile){
        // get filename
        filename=getFilename();
        // open file in read mode
        output=fopen(filename,"w");
    }
    // switch based on the choice
    switch(choice){
        case 1:{
            displayAvgNoOfTripsPerHour(output,tripData,tripCount);
            break;
        }
        case 2:{
            displayHourWithMaxTripCount(output,tripData,tripCount);
            break;
        }
        case 3:{
            displayNoOfTripsPerHour(output, tripData,tripCount);
            break;
        }
        case 4:{
            displayAvgDurationOfTrips(output, tripData,tripCount);
            break;
        }
        case 5:{
            displayLongestDurationTrip(output, tripData,tripCount);
            break;
        }
        case 6:{
            display5BusiestStartingStation(output, tripData,tripCount);

            break;
        }
        case 7:{
            display5BusiestEndingStation(output, tripData,tripCount);
            break;
        }
        case 8:{
            displayPercentageOfMembershipType(output, tripData,tripCount);
            break;
        }
    }
}

// This function is used to partition trip data in ascending order of bike ID
int partition (Trip *tripData, int low, int high){
    // determine pivot element
    int pivot = tripData[high].bikeId;
    // determine index of smaller element
    int i = (low - 1);

    // loop for j from low to hight
    for (int j = low; j <= high - 1; j++){
        // if current trip data is smaller than the pivot
        if (tripData[j].bikeId < pivot){
            // increment index of smaller element
            i++;

            // swap ith and jth trip data
            Trip temp=tripData[i];
            tripData[i]=tripData[j];
            tripData[j]=temp;

        }
    }
    // swap (i+1)th and high th trip data
    Trip temp=tripData[i+1];
    tripData[i+1]=tripData[high];
    tripData[high]=temp;

    return (i + 1);
}

// This function is used to sort the trip data in ascending order of bike ID
void quickSort(Trip *tripData, int low, int high){
    // if low < high
    if (low < high){
        // determine index of pivot element
        int pi = partition(tripData, low, high);
        // divide the array into two parts
        quickSort(tripData, low, pi - 1);
        quickSort(tripData, pi + 1, high);
    }
}

// This function is used to determine index of bike with particular bike ID
int getBikeIndex(Bike *bikeData,int bikeCount, int bikeId){
    int foundBikeIndex=-1;
    // determine index of bike with particular bike ID
    for(int j=0;j<bikeCount;j++){
        if(bikeData[j].bikeId==bikeId){
            foundBikeIndex=j;
        }
    }
    // return found bike index
    return foundBikeIndex;
}

// This function is used to determine unique bike IDs
int determineUniqueBikeID(Trip *tripData,int tripCount, Bike *bikeData){
    // sort the trip data in ascending order of bike ID
    quickSort(tripData,0,tripCount-1);
    int bikeCount=0;

    // loop for each trip count
    for(int i=0;i<tripCount;i++){
        // determine index of bike with particular bike ID
        int foundBikeIndex=getBikeIndex(bikeData, bikeCount,tripData[i].bikeId);

        // if index is not found
        if(foundBikeIndex==-1){
            // store bike ID in bike array
            bikeData[bikeCount].bikeId=tripData[i].bikeId;
            bikeData[bikeCount].endStationId=0;
            bikeData[bikeCount].maintenanceFlag=0;
            bikeData[bikeCount].numTrips=0;
            bikeData[bikeCount].totalDuration=0;
            bikeData[bikeCount].totalMinutes=0;

            // increment bike count by 1
            bikeCount++;
        }
    }

    return bikeCount;
}

// This function is used to extract the details of bike from trips
int getBike(Trip *tripData,int tripCount, Bike *bikeData){
    // determine list of unique bike ID
    int bikeCount=determineUniqueBikeID(tripData,tripCount,bikeData);

    // loop for each trips
    for(int i=0;i<tripCount;i++){
        // determine index of current trip bike ID
        int foundBikeIndex=getBikeIndex(bikeData,bikeCount,tripData[i].bikeId);

        // if bike is found
        if(foundBikeIndex>=0){
            // update bike details
            bikeData[foundBikeIndex].endStationId=tripData[i].endStationId;
            bikeData[foundBikeIndex].numTrips+=1;
            bikeData[foundBikeIndex].totalDuration+=tripData[i].duration;
            bikeData[foundBikeIndex].totalMinutes+=getMinuteFromMS(tripData[i].duration);
        }
    }
    return bikeCount;
}

// This function is used to display print menu
void displayPrintMenu(){
    printf("\nPrint Menu");
    printf("\n-----------");
    printf("\n1) Print All the Trip Data");
    printf("\n2) Print All the Bike Inventory");
    printf("\n3) Print the CEO Report");
    printf("\n4) Print one Bike");
}

// This function is used to display list of trip details
void displayTripData(FILE *output,Trip *tripData, int tripCount){
    // display the header
    fprintf(output, "%-15s %-15s %-15s %-15s %-15s %-15s %-15s\n",
               "Membership Type","Start Station ID","End Station ID",
               "Bike ID", "Duration", "Start Hour", "Start Minute");
    fflush(output);
    // loop for each trips
    for(int i=0;i<tripCount;i++){
        // display ith trip details
        fprintf(output, "%-15s %-15d %-15d %-15d %-15d %-15d %-15d\n",
               tripData[i].membershipType,tripData[i].startStationId,tripData[i].endStationId,
               tripData[i].bikeId,tripData[i].duration,tripData[i].startHr,tripData[i].startMin);
        fflush(output);
    }
}

// This function is used to display list of bike details
void displayAllBikes(FILE *output,Bike *bikeData, int bikeCount){
    // display the header
    fprintf(output, "%-15s %-15s %-15s %-15s %-15s %-15s\n",
               "Bike ID", "End Station ID", "Number of Trips", "Total Duration", "Total Minutes","Maintenance");
    fflush(output);
    // loop for each bike
    for(int i=0;i<bikeCount;i++){
        // display ith bike details
        fprintf(output, "%-15d %-15d %-15d %-15d %-15.2f %s\n",
               bikeData[i].bikeId,bikeData[i].endStationId,bikeData[i].numTrips,
               bikeData[i].totalDuration,bikeData[i].totalMinutes, (bikeData[i].maintenanceFlag?"Yes":"No"));
        fflush(output);
    }
}

// This function is used to determine top 10 bike with longest duration
int *getBikeWithLongestDuration(Bike* bikeData, int bikeCount){
    int* bikeWithLongestDuration=(int *)malloc(10*sizeof(int));

    // sort bike array in descending order of total minutes
    for(int i=0;i<bikeCount;i++){
        for(int j=i+1;j<bikeCount;j++){
            if(bikeData[i].totalMinutes<bikeData[j].totalMinutes){
                Bike temp=bikeData[i];
                bikeData[i]=bikeData[j];
                bikeData[j]=temp;
            }
        }
    }

    // determine and return top 10 bike with longest duration
    for(int i=0;i<10;i++){
        bikeWithLongestDuration[i]=bikeData[i].bikeId;
    }
    return bikeWithLongestDuration;
}


// This function is used to display top 10 bike with longest duration
void display10BikeWithLongestDuration(FILE *output,Bike *bikeData,int bikeCount){
    if(bikeCount>0){
        // determine top 10 bike with longest duration
        int *bikeWithLongestDuration = getBikeWithLongestDuration(bikeData,bikeCount);
        // display list of top 10 bike with Longest Duration in Minutes
        fprintf(output,"The 10 Bike with Longest Duration in Minutes\n");
        fflush(output);
        fprintf(output,"%-15s%s\n","Bike ID","Total Duration in Minutes");
        fflush(output);
        for(int i=0;i<10;i++){
            Bike bike=bikeData[getBikeIndex(bikeData,bikeCount,bikeWithLongestDuration[i])];
            fprintf(output,"%-15d%.2f\n",bike.bikeId,bike.totalMinutes);
            fflush(output);
        }
    }
    else{
        // display error message
        fprintf(output,"No bike exist.\n");
        fflush(output);
    }
}
void display10BikeWithLongestDurationLoc(FILE *output,Bike *bikeData,int bikeCount){
    if(bikeCount>0){
        // determine top 10 bike with longest duration
        int *bikeWithLongestDuration = getBikeWithLongestDuration(bikeData,bikeCount);

        // determine top 10 bike with longest duration and its end station
        fprintf(output,"The 10 Bike with Longest Duration and its end station\n");
        fflush(output);
        fprintf(output,"%-15s%s\n","Bike ID","End Station");
        fflush(output);
        for(int i=0;i<10;i++){
            Bike bike=bikeData[getBikeIndex(bikeData,bikeCount,bikeWithLongestDuration[i])];
            fprintf(output,"%-15d%d\n",bike.bikeId,bike.endStationId);
            fflush(output);
        }
    }
    else{
        // display error message
        fprintf(output,"No bike exist.\n");
        fflush(output);
    }
}

// This function is used to determine top 10 bike with most trips
int *getBikeWithMostTrips(Bike* bikeData, int bikeCount){
    int* bikeWithMostTrips=(int *)malloc(10*sizeof(int));

    // sort bike in descending order based on number of trips
    for(int i=0;i<bikeCount;i++){
        for(int j=i+1;j<bikeCount;j++){
            if(bikeData[i].numTrips<bikeData[j].numTrips){
                Bike temp=bikeData[i];
                bikeData[i]=bikeData[j];
                bikeData[j]=temp;
            }
        }
    }

    // determine and return top 10 bike with most trips
    for(int i=0;i<10;i++){
        bikeWithMostTrips[i]=bikeData[i].bikeId;
    }
    return bikeWithMostTrips;
}

// This function is used to display top 10 bike with most trips
void display10BikeWithMostTrips(FILE *output,Bike *bikeData,int bikeCount){
    // if number of bikes is greater than 0
    if(bikeCount>0){
        // determine list of bike with most trips
        int *bikeWithMostTrips = getBikeWithMostTrips(bikeData,bikeCount);

        // display top 10 bike with most trips
        fprintf(output,"The 10 Bike with Most Trips\n");
        fflush(output);
        for(int i=0;i<10;i++){
            fprintf(output,"%d\n",bikeWithMostTrips[i]);
            fflush(output);
        }
    }
    else{
        // display error message
        fprintf(output,"No bike exist.\n");
        fflush(output);
    }
}

// This function is used to display bike with 2 or less trips
void displayBikeWith2OrLessTrips(FILE *output,Bike *bikeData,int bikeCount){
    if(bikeCount>0){
        fprintf(output,"Bike with 2 or less Trips\n");
        fflush(output);

        // loop for each bike
        for(int i=0;i<bikeCount;i++){
            // if number of trips is atmost 2
            if(bikeData[i].numTrips<=2){
                fprintf(output,"%d\n",bikeData[i].bikeId);
                fflush(output);
            }
       }
    }
    else{
        // display error message
        fprintf(output,"No bike exist.\n");
        fflush(output);
    }
}

// This function is used to display the start station and number of trip from it by the particular bike ID
int getStartStation(Trip *tripData,int tripCount,int bikeId,int *startStations,int *numberOfTrips){
    int startStationCount=0;
    // loop for each trip
    for(int i=0;i<tripCount;i++){
        // if bikeID matches
        if(tripData[i].bikeId==bikeId){
            int foundeIndex=-1;
            // increment number of trip by 1 if the start station is found already
            for(int j=0;j<startStationCount;j++){
                if(startStations[j]==tripData[i].startStationId){
                    foundeIndex=j;
                    numberOfTrips[j]+=1;
                }
            }

            // if the start station is not found
            if(foundeIndex==-1){
                // store the start station and number of trips
                startStations[startStationCount]=tripData[i].startStationId;
                numberOfTrips[startStationCount]=1;

                // increment number of start station by 1
                startStationCount++;
            }
        }
    }
    return startStationCount;
}

// This function is used to determine the list of start station for bike with 2 or less tan 2 trips
void displayStartStationTripWithLessTripsBike(FILE *output,Trip *tripData,int tripCount, Bike *bikeData,int bikeCount){
    if(bikeCount>0){
        fprintf(output,"Bike with 2 or less Trips\n");
        fflush(output);
        // loop for each bike
        for(int i=0;i<bikeCount;i++){
            // if number of trips is at most 2
            if(bikeData[i].numTrips<=2){
                fprintf(output,"List of Start Station and number of trips to them by bike %d\n",bikeData[i].bikeId);
                fflush(output);
                fprintf(output,"%-15s %s\n", "Start Station","Number of Trips");
                fflush(output);
                int *startStations=(int *)malloc(2*sizeof(int));
                int *numberOfTrips=(int *)malloc(2*sizeof(int));
                // determine and display start station with number of trips
                int startStationCount=getStartStation(tripData,tripCount,bikeData[i].bikeId,startStations,numberOfTrips);
                for(int j=0;j<startStationCount;j++){
                    fprintf(output,"%-15d %d\n", startStations[j],numberOfTrips[j]);
                    fflush(output);
                }
            }
        }
    }
    else{
        // display error message
        fprintf(output,"No bike exist.\n");
        fflush(output);
    }
}

// This function is used to print ceo report
void printCEOReport(FILE *output,Trip *tripData, int tripCount, Bike *bikeData, int bikeCount){
    fprintf(output,"CEO Report\n");
    fflush(output);

    // display trip metrics
    fprintf(output,"Trip Metrics are given below:\n");
    fflush(output);

    displayAvgNoOfTripsPerHour(output,tripData,tripCount);
    displayHourWithMaxTripCount(output,tripData,tripCount);
    displayNoOfTripsPerHour(output, tripData,tripCount);
    displayAvgDurationOfTrips(output, tripData,tripCount);
    displayLongestDurationTrip(output, tripData,tripCount);
    display5BusiestStartingStation(output, tripData,tripCount);
    display5BusiestEndingStation(output, tripData,tripCount);
    displayPercentageOfMembershipType(output, tripData,tripCount);

    // display bike metrics
    fprintf(output,"Bike Metrics are given below:\n");
    fflush(output);

    display10BikeWithLongestDuration(output,bikeData,bikeCount);
    display10BikeWithLongestDurationLoc(output,bikeData,bikeCount);
    display10BikeWithMostTrips(output,bikeData,bikeCount);
    displayBikeWith2OrLessTrips(output,bikeData,bikeCount);
    displayStartStationTripWithLessTripsBike(output,tripData,tripCount,bikeData,bikeCount);

    printf("CEO Report is generated Successfully!");
}

// This function is used to display the details of bike with particular bike ID
void displayBike(FILE *output,Bike *bikeData, int bikeCount,int bikeID){
    // determine index of bike with particular bike ID
    int bikeIndex=getBikeIndex(bikeData,bikeCount,bikeID);
    // if bike ID exist
    if(bikeIndex!=-1){
        // display details of bike
        fprintf(output, "Bike ID: %d\n",bikeData[bikeIndex].bikeId);
        fprintf(output, "End Station ID: %d\n",bikeData[bikeIndex].endStationId);
        fprintf(output, "Number of Trips: %d\n",bikeData[bikeIndex].numTrips);
        fprintf(output, "Total Duration: %d\n",bikeData[bikeIndex].totalDuration);
        fprintf(output, "Total Minutes: %.2f\n",bikeData[bikeIndex].totalMinutes);
        fprintf(output, "Maintenance: %s\n",(bikeData[bikeIndex].maintenanceFlag?"Yes":"No"));
    }
    else{
        // display error message
        fprintf(output,"bike ID does not exist.\n");
        fflush(output);
    }
}

// This function is used to perform print menu option
void printMenu(Trip *tripData, int tripCount, Bike *bikeData, int bikeCount){
    // display print menu
    displayPrintMenu();

    // prompt and get valid choice
    int choice=getValidChoice(1,4);
    printf("\n");
    int bikeID;

    // if choice is 4
    if(choice==4){
        // prompt and get bike ID
        printf("Enter the bike ID: ");
        scanf("%d",&bikeID);
    }
    int doWriteInFile=1;

    // if choice is not 3
    if(choice!=3){
        // check if user wills to write output to file
        doWriteInFile=getYorN("Do require to write the result to an output file? (y or n): ");
    }
    char *filename="";
    FILE *output=stdout;

    // if do want to write in file
    if(doWriteInFile){
        // get the filename
        filename=getFilename();
        // open the file in write mode
        output=fopen(filename,"w");
    }
    // switch based on user choice
    switch(choice){
        case 1:{
            displayTripData(output, tripData,tripCount);
            break;
        }
        case 2:{
            displayAllBikes(output, bikeData,bikeCount);
            break;
        }
        case 3:{
            printCEOReport(output, tripData,tripCount, bikeData,bikeCount);
            break;
        }
        case 4:{
            displayBike(output, bikeData,bikeCount,bikeID);
            break;
        }
    }
}

// This function is used to flag bike for maintenance
void flagBikeForMaintenance(Bike *bikeData,int bikeCount){
    // determine the list of bike with longest duration
    int *bikeWithLongestDuration = getBikeWithLongestDuration(bikeData,bikeCount);

    // update the maintenance flag of bike to 1
    for(int i=0;i<10;i++){
        bikeData[getBikeIndex(bikeData,bikeCount,bikeWithLongestDuration[i])].maintenanceFlag=1;
    }

    // display the success message
    printf("Maintenance flag is updated successfully");
}

// This function is used to add bike to bike array
Bike *addBike(Bike *bikeData,int *bikeCount){
    int bikeID;

    // prompt and get bike ID
    printf("Enter the Bike ID: ");
    scanf("%d",&bikeID);

    // determine index of bike with particular ID
    int bikeIndex=getBikeIndex(bikeData,*bikeCount,bikeID);

    // if bike ID does not exist already
    if(bikeIndex==-1){
        // store bike ID at last position
        bikeData[*bikeCount].bikeId=bikeID;
        bikeData[*bikeCount].endStationId=0;
        bikeData[*bikeCount].maintenanceFlag=0;
        bikeData[*bikeCount].numTrips=0;
        bikeData[*bikeCount].totalDuration=0;
        bikeData[*bikeCount].totalMinutes=0;

        // increment number of bikes by 1
        *bikeCount += 1;
        printf("Bike added successfully\n");
    }
    else{
        // display bike already exist
        printf("Bike ID already exist\n");
    }
    return bikeData;
}

// This function is used to remove bike with particular ID
void removeBike(Bike *bikeData,int *bikeCount){
    int bikeID;
    // prompt and get bike ID
    printf("Enter the Bike ID: ");
    scanf("%d",&bikeID);

    // determine index of bike with particular bike ID
    int bikeIndex=getBikeIndex(bikeData,*bikeCount,bikeID);

    // if bike ID exist
    if(bikeIndex!=-1){
        // remove bike from bike array
        for(int i=bikeIndex;i<*bikeCount-1;i++){
            bikeData[i]=bikeData[i+1];
        }
        // decrement bike count by 1
        *bikeCount -= 1;
        printf("Bike removed successfully\n");
    }
    else{
        // display bike does not exist
        printf("Bike ID does not exist\n");
    }
}

int main(){
    int tripCount=0;
    Trip* tripData;

    int bikeCount=0;
    Bike bikeData[40000];
    bikeData[0].bikeId=1234;

    int loop=1;
    // loop until user wishes to exit
    while(loop){
        // display main menu
        displayMainMenu();
        // prompt and get choice
        int choice=getValidChoice(1,7);

        // switch based on choice
        switch(choice){
            case 1:
                tripData=readTripData(&tripCount);
                bikeCount=getBike(tripData,tripCount, bikeData);

                break;
            case 2:
                runMetrics(tripData,tripCount);
                break;
            case 3:
                printMenu(tripData,tripCount,bikeData,bikeCount);
                break;
            case 4:
                flagBikeForMaintenance(bikeData,bikeCount);
                break;
            case 5:
                addBike(bikeData,&bikeCount);
                break;
            case 6:
                removeBike(bikeData,&bikeCount);
                break;
            case 7:
                loop=0;
                break;
        }
        printf("\n");
    }
    // display exit message
    printf("\nSoftware is Exiting..");

    return 0;
}
