#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MAX_USERS 10000 // Adjust the maximum number of users as needed

// Define the structure for user positions
struct User {
    char name[100];
    double latitude, longitude, altitude;
    struct timespec entryTime;
};

// Define the structure for relative distances
struct Distance {
    char name[100];
    double distance;
};

// Function to scan user position data
void scanUser(struct User *user) {
    printf("Enter reference user name: ");
    scanf("%s", user->name);

    printf("Enter reference user latitude: ");
    scanf("%lf", &user->latitude);

    printf("Enter reference user longitude: ");
    scanf("%lf", &user->longitude);

    printf("Enter reference user altitude: ");
    scanf("%lf", &user->altitude);

    // Record entry time
    clock_gettime(CLOCK_REALTIME, &user->entryTime);
}

// Function to scan user data from a file
void scanUserFromFile(FILE *file, struct User *user) {
    fscanf(file, "%s %lf %lf %lf", user->name, &user->latitude, &user->longitude, &user->altitude);
    // Set entry time to current time
    clock_gettime(CLOCK_REALTIME, &user->entryTime);
}

// Function to calculate distance between two users
double calculateDistance(struct User refUser, struct User otherUser) {
    double latDiff = refUser.latitude - otherUser.latitude;
    double longDiff = refUser.longitude - otherUser.longitude;
    double altDiff = refUser.altitude - otherUser.altitude;

    return sqrt(latDiff * latDiff + longDiff * longDiff + altDiff * altDiff);
}

// Function to find the closest user
struct Distance findClosestUser(struct User ourUser, struct User *otherUsers, int numUsers) {
    struct Distance closestUser;
    closestUser.distance = INFINITY; // Initialize to positive infinity

    for (int i = 0; i < numUsers; ++i) {
        double distance = calculateDistance(ourUser, otherUsers[i]);

        if (distance < closestUser.distance) {
            closestUser.distance = distance;
            strcpy(closestUser.name, otherUsers[i].name);
        }
    }

    return closestUser;
}

// Find the index of the closest user in the .txt file
int findClosestUserIndex(struct User ourUser, struct User *otherUsers, int numUsers) {
    struct Distance closestUser;
    closestUser.distance = INFINITY;
    int closestIndex = 0;

    for (int i = 0; i < numUsers; ++i) {
        double distance = calculateDistance(ourUser, otherUsers[i]);

        if (distance < closestUser.distance) {
            closestUser.distance = distance;
            closestIndex = i;
        }
    }

    return closestIndex;
}

// Function to calculate elapsed time in nanoseconds
long long calculateElapsedTime(struct timespec startTime, struct timespec endTime) {
    return (endTime.tv_sec - startTime.tv_sec) * 1e9 + (endTime.tv_nsec - startTime.tv_nsec);
}

int main() {
    int mode;
    printf("Choose mode:\n");
    printf("1. Manual entry of other users and their data\n");
    printf("2. Use a prewritten text file containing the entries of other users and their data\n");
    scanf("%d", &mode);

    if (mode == 1) {
        int numOtherUsers;
        printf("Enter the number of other users: ");
        scanf("%d", &numOtherUsers);

        struct User ourUser;
        scanUser(&ourUser);

        // Fixed-size array for other users
        struct User otherUsers[MAX_USERS];

        for (int i = 0; i < numOtherUsers; ++i) {
            printf("Enter data for other user %d:\n", i + 1);
            scanUser(&otherUsers[i]);
        }

        printf("Actual distances from our user:\n");
        for (int i = 0; i < numOtherUsers; ++i) {
            double distance = calculateDistance(ourUser, otherUsers[i]);
            printf("Distance to %s: %.2lf\n", otherUsers[i].name, distance);
        }

        struct Distance closestUser = findClosestUser(ourUser, otherUsers, numOtherUsers);
        int closestIndex = findClosestUserIndex(ourUser, otherUsers, numOtherUsers);
        printf("Nearest user to reference user:\nUser: %s\n", closestUser.name);
        printf("Latitude: %.2lf\n", otherUsers[closestIndex].latitude);
        printf("Longitude: %.2lf\n", otherUsers[closestIndex].longitude);
        printf("Altitude: %.2lf\n", otherUsers[closestIndex].altitude);

        // Calculate and print elapsed time
        struct timespec currentTime;
        clock_gettime(CLOCK_REALTIME, &currentTime);
        long long elapsedTime = calculateElapsedTime(ourUser.entryTime, currentTime);
        printf("Elapsed time since entering coordinates: %lld nanoseconds (approx. %lld seconds)\n", elapsedTime, elapsedTime/1000000000);
    }
    else if (mode == 2) {
        char filePath[500];
        printf("Enter the path of the input file: ");
        scanf("%s", filePath);

        FILE *file = fopen(filePath, "r");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        int numOtherUsers;
        fscanf(file, "%d", &numOtherUsers);

        struct User ourUser;
        scanUser(&ourUser);

        // Fixed-size array for other users
        struct User otherUsers[MAX_USERS];

        for (int i = 0; i < numOtherUsers; ++i) {
            scanUserFromFile(file, &otherUsers[i]);
        }

        fclose(file);

        printf("Actual distances from our user:\n");
        for (int i = 0; i < numOtherUsers; ++i) {
            double distance = calculateDistance(ourUser, otherUsers[i]);
            printf("Distance to %s: %.2lf\n", otherUsers[i].name, distance);
        }

        struct Distance closestUser = findClosestUser(ourUser, otherUsers, numOtherUsers);
        printf("Nearest user to reference user:\nUser: %s\n", closestUser.name);
        printf("Latitude: %.2lf\n", otherUsers[findClosestUserIndex(ourUser, otherUsers, numOtherUsers)].latitude);
        printf("Longitude: %.2lf\n", otherUsers[findClosestUserIndex(ourUser, otherUsers, numOtherUsers)].longitude);
        printf("Altitude: %.2lf\n", otherUsers[findClosestUserIndex(ourUser, otherUsers, numOtherUsers)].altitude);

        // Calculate and print elapsed time
        struct timespec currentTime;
        clock_gettime(CLOCK_REALTIME, &currentTime);
        long long elapsedTime = calculateElapsedTime(ourUser.entryTime, currentTime);
        printf("Elapsed time since entering referenece user's coordinates: %lld nanoseconds (approx. %lld seconds)\n", elapsedTime, elapsedTime/1000000000);
    }
   
    return 0;
}