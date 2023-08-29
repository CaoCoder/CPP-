/*
    Data structure and function implementations for a 
    dictionary module intended to allow lookup
    for particular values.
*/
#include "dictionary.h"
#include "record_struct.h"
#include "record_struct.c"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NUMERIC_BASE 10
#define KEY_FIELD 7
#define NOTFOUND "NOTFOUND"
#define NOTDOUBLE (-1)
#define MAXPRECISION (-2)
#define NUM_FIELDS 14

#define FIELDLOOKUPFAILURE (-1)

#define INDEXINITIAL 1

// Field names by index.
static char *fieldNames[] = {"census_year", "block_id", "property_id", 
    "base_property_id", "building_address", "clue_small_area", "business_address",
    "trading_name", "industry_code", "industry_description", "seating_type",
    "number_of_seats", "longitude", "latitude"};

/* 
Field double precision by index. -1 signifies that the value should not
be printed as a double value.
*/
static int fieldPrecision[] = {NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, 
    NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, NOTDOUBLE,
    NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, NOTDOUBLE,
    NOTDOUBLE, 5, 5};

struct queryResult {
    char *searchString;
    int numRecords;
    double closestValue;
    struct data **records;
};

/* CSV records. */
struct data;
struct data {
    int census_year;
    int block_id;
    int property_id;
    int base_property_id;
    char *building_address;
    char *clue_small_area;
    char *business_address;
    char *trading_name;
    int industry_code;
    char *industry_description;
    char *seating_type;
    int number_of_seats;
    double longitude;
    double latitude;
};

/* A node in the dictionary, used to allow quick lookup. */
struct dictionaryNode;

struct dictionaryNode{
    struct data *record;
    struct dictionaryNode *next;
};

struct index;

struct index {
    int valueCount;
    struct dictionaryNode **nodes;
};

/* Dictionary - concrete data structure is linked list. */
struct dictionary {
    struct dictionaryNode *head;
    struct dictionaryNode *tail;
    struct index **indices;
};

/* Reads a given string as an integer and returns the integer. */
int readIntField(char *fieldString);

/* Reads a given string as a string and returns a copy of the string. */
char *readStringField(char *fieldString);

/* Reads a given string as a double value and returns the double value. */
double readDoubleField(char *fieldString);

int readIntField(char *fieldString){
    return strtol(fieldString, NULL, NUMERIC_BASE);
}

char *readStringField(char *fieldString){
    char *str = strdup(fieldString);
    assert(str);
    return str;
}

double readDoubleField(char *fieldString){
    return strtod(fieldString, NULL);
}

/* Sets the relevant field. */
void setField(struct data *record, int fieldIndex, char *fieldVal);

void setField(struct data *record, int fieldIndex, char *fieldVal){
    switch(fieldIndex){
        case 0:
            record->census_year = readIntField(fieldVal);
            break;
        case 1:
            record->block_id = readIntField(fieldVal);
            break;
        case 2:
            record->property_id = readIntField(fieldVal);
            break;
        case 3:
            record->base_property_id = readIntField(fieldVal);
            break;
        case 4:
            record->building_address = readStringField(fieldVal);
            break;
        case 5:
            record->clue_small_area = readStringField(fieldVal);
            break;
        case 6:
            record->business_address = readStringField(fieldVal);
            break;
        case 7:
            record->trading_name = readStringField(fieldVal);
            break;
        case 8:
            record->industry_code = readIntField(fieldVal);
            break;
        case 9:
            record->industry_description = readStringField(fieldVal);
            break;
        case 10:
            record->seating_type = readStringField(fieldVal);
            break;
        case 11:
            record->number_of_seats = readIntField(fieldVal);
            break;
        case 12:
            record->longitude = readDoubleField(fieldVal);
            break;
        case 13:
            record->latitude = readDoubleField(fieldVal);
            break;
        default:
            fprintf(stderr, "%d: Unhandled field number %d\n", __LINE__, 
                fieldIndex);
            assert(fieldIndex >= 0 && fieldIndex < NUM_FIELDS);
    }
}

/* Returns the double value for the given field index. */
double getDoubleVal(struct data *record, int fieldIndex);

double getDoubleVal(struct data *record, int fieldIndex){
    switch(fieldIndex){
        case 12:
            return record->longitude;
        case 13:
            return record->latitude;
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            fprintf(stderr, "%d: Unhandled non-double field number %d\n", __LINE__, 
                fieldIndex);
            assert(0);
            break;
        default:
            fprintf(stderr, "%d: Unhandled field number %d\n", __LINE__, 
                fieldIndex);
            assert(fieldIndex >= 0 && fieldIndex < NUM_FIELDS && 0);
    }
    return FIELDLOOKUPFAILURE;
}

/* Prints a given value. */
void printIntField(FILE *f, int value);

/* Prints a given value. */
void printStringField(FILE *f, char *value);

/* Prints a given value. */
void printDoubleField(FILE *f, double value, int fieldPrecision);

void printIntField(FILE *f, int value){
    fprintf(f, "%d", value);
}

void printStringField(FILE *f, char *value){
    fprintf(f, "%s", value);
}

void printDoubleField(FILE *f, double value, int fieldPrecision){
    fprintf(f, "%.*f", fieldPrecision, value);
}

/* Prints the relevant field. */
void printField(FILE *f, struct data *record, int fieldIndex);

void printField(FILE *f, struct data *record, int fieldIndex){
    switch(fieldIndex){
                case 0:
            printIntField(f, record->census_year);
            break;
        case 1:
            printIntField(f, record->block_id);
            break;
        case 2:
            printIntField(f, record->property_id);
            break;
        case 3:
            printIntField(f, record->base_property_id);
            break;
        case 4:
            printStringField(f, record->building_address);
            break;
        case 5:
            printStringField(f, record->clue_small_area);
            break;
        case 6:
            printStringField(f, record->business_address);
            break;
        case 7:
            printStringField(f, record->trading_name);
            break;
        case 8:
            printIntField(f, record->industry_code);
            break;
        case 9:
            printStringField(f, record->industry_description);
            break;
        case 10:
            printStringField(f, record->seating_type);
            break;
        case 11:
            printIntField(f, record->number_of_seats);
            break;
        case 12:
            printDoubleField(f, record->longitude, fieldPrecision[fieldIndex]);
            break;
        case 13:
            printDoubleField(f, record->latitude, fieldPrecision[fieldIndex]);
            break;
        default:
            fprintf(stderr, "%d: Unhandled field number %d\n", __LINE__, 
                fieldIndex);
            assert(fieldIndex >= 0 && fieldIndex < NUM_FIELDS);
    }
}

struct dictionary *newDict(){
    struct dictionary *ret = (struct dictionary *) 
        malloc(sizeof(struct dictionary));
    assert(ret);
    ret->head = NULL;
    ret->tail = NULL;
    ret->indices = NULL;
    return ret;
}

/* Read a record into a typed data structure. */
struct data *readRecord(struct csvRecord *record);

struct data *readRecord(struct csvRecord *record){
    struct data *ret = (struct data *) malloc(sizeof(struct data));
    assert(ret);
    assert(record->fieldCount == NUM_FIELDS);
    for(int i = 0; i < NUM_FIELDS; i++){
        setField(ret, i, record->fields[i]);
    }
    return ret;
};

void insertRecord(struct dictionary *dict, struct csvRecord *record){
    if(! dict){
        return;
    }
    struct dictionaryNode *newNode = (struct dictionaryNode *) 
        malloc(sizeof(struct dictionaryNode));
    assert(newNode);
    newNode->record = readRecord(record);
    newNode->next = NULL;

    if(! (dict->head)){
        /* First insertion, insert new node as head and tail. */
        dict->head = newNode;
        dict->tail = newNode;
    } else {
        /* Insert at tail. */
        dict->tail->next = newNode;
        dict->tail = newNode;
    }
}

/* Search for a given key in the dictionary. */
struct queryResult *lookupRecord(struct dictionary *dict, char *query){
    int numRecords = 0;
    struct data **records = NULL;

    /* Iterate over all records and collect all matching records. */
    struct dictionaryNode *current = dict->head;
    while(current){
        if(strcmp(current->record->trading_name, query) == 0){
            /* NB: Here we use a time inefficient algorithm as we assume
                the number of records matching is likely to be small. May fix 
                later. */
            /* Match. */
            records = (struct data **) realloc(records, 
                sizeof(struct data *) * (numRecords + 1));
            assert(records);
            records[numRecords] = current->record;
            numRecords++;
        }
        current = current->next;
    }

    struct queryResult *qr = (struct queryResult *) 
        malloc(sizeof(struct queryResult));
    assert(qr);

    qr->searchString = strdup(query);
    assert(qr->searchString);
    qr->numRecords = numRecords;
    qr->records = records;
    return qr;
}

/* Output the given query result. */
void printQueryResult(struct queryResult *r, FILE *summaryFile, 
    FILE *outputFile, int stage){
    /* Print summary. */
    if(stage == LOOKUPSTAGE){
        if(r->numRecords == 0){
            fprintf(summaryFile, "%s --> %s\n", r->searchString, NOTFOUND);
        } else {
            fprintf(summaryFile, "%s --> %d\n", r->searchString, r->numRecords);
        }
    } else {
        exit(EXIT_FAILURE);
    }
    /* Print details. */
    fprintf(outputFile, "%s\n", r->searchString);
    for(int i = 0; i < r->numRecords; i++){
        fprintf(outputFile, "--> ");
        for(int j = 0; j < NUM_FIELDS; j++){
            fprintf(outputFile, "%s: ", fieldNames[j]);
            printField(outputFile, r->records[i], j);
            fprintf(outputFile, " || ");
        }
        fprintf(outputFile, "\n");
    }
}

/* Builds an index for the given (double) field in the data structure. */
struct index *buildDoubleIndex(struct dictionary *dict, int fieldIndex);

struct index *buildDoubleIndex(struct dictionary *dict, int fieldIndex){
    struct index *index = (struct index *) malloc(sizeof(struct index));
    assert(index);
    int valueCount = 0;
    struct dictionaryNode **nodes = NULL;
    
    int nodeSpace = 0;

    nodes = (struct dictionaryNode **) malloc(sizeof(struct dictionaryNode *) * INDEXINITIAL);
    assert(nodes);
    nodeSpace = INDEXINITIAL;

    /* Insertion sort. */
    struct dictionaryNode *current = dict->head;

    while(current){
        if(valueCount == nodeSpace){
            nodeSpace *= 2;
            nodes = (struct dictionaryNode **) realloc(nodes, 
                sizeof(struct dictionaryNode *) * nodeSpace);
            assert(nodes);
        }
        nodes[valueCount] = current;
        for(int j = valueCount - 1; j >= 0; j--){
            if(getDoubleVal(current->record, fieldIndex) < getDoubleVal(nodes[j]->record, fieldIndex)){
                nodes[j + 1] = nodes[j];
                nodes[j] = current;
            } else {
                /* Sorted. */
                break;
            }
        }
        valueCount++;
        current = current->next;
    }

    index->valueCount = valueCount;
    index->nodes = nodes;

    return index;
}

/* Searches for the closest value using the index built for the field. If an index
    has not yet been built, it is  */
struct queryResult *searchClosestDouble(struct dictionary *dict, char *query, 
    int fieldIndex){
    if(! dict->indices){
        dict->indices = (struct index **) malloc(sizeof(struct index *) * NUM_FIELDS);
        assert(dict->indices);
        for(int i = 0; i < NUM_FIELDS; i++){
            (dict->indices)[i] = NULL;
        }
    }
    if(! dict->indices[fieldIndex]){
        dict->indices[fieldIndex] = buildDoubleIndex(dict, fieldIndex);
    }

    double searchVal = strtod(query, NULL);

    struct index *index = dict->indices[fieldIndex];
    int highIdx = index->valueCount - 1;
    int lowIdx = 0;
    while(highIdx > lowIdx){
        int mid = (highIdx + lowIdx) / 2;
        if(getDoubleVal(index->nodes[mid]->record, fieldIndex) < searchVal){
            lowIdx = mid + 1;
        } else if (getDoubleVal(index->nodes[mid]->record, fieldIndex) > searchVal){
            highIdx = mid - 1;
        } else {
            // Exact match. Terminate early.
            lowIdx = mid;
            highIdx = mid;
        }
    }
    int currentIdx = (highIdx + lowIdx) / 2;
    if(currentIdx < 0){
        currentIdx = 0;
    } else if(currentIdx >= index->valueCount){
        currentIdx = index->valueCount - 1;
    }
    double closestProximity = getDoubleVal(index->nodes[currentIdx]->record, fieldIndex) - searchVal;
    /* Check surrounding values */
    if(closestProximity < 0){
        /* searchVal is larger than the current index. */
        closestProximity = -closestProximity;
        /* Check if value higher is closer. */
        if((currentIdx + 1) < index->valueCount){
            double candidateProximity = getDoubleVal(index->nodes[currentIdx + 1]->record, fieldIndex) 
                - searchVal;
            if(candidateProximity < 0){
                candidateProximity = -candidateProximity;
            }
            if(candidateProximity < closestProximity){
                currentIdx = currentIdx + 1;
            }
        }
    } else {
        /* searchVal is smaller or equal to the current index. */
        if((currentIdx - 1) >= 0){
            double candidateProximity = getDoubleVal(index->nodes[currentIdx - 1]->record, fieldIndex) 
                - searchVal;
            if(candidateProximity < 0){
                candidateProximity = -candidateProximity;
            }
            if(candidateProximity < closestProximity){
                currentIdx = currentIdx - 1;
            }
        }
    }
    
    struct queryResult *qr = (struct queryResult *) 
        malloc(sizeof(struct queryResult));
    assert(qr);

    int numRecords = 1;

    struct data **records = (struct data **) 
        malloc(sizeof(struct data *) * (numRecords));
    assert(records);

    records[0] = index->nodes[currentIdx]->record;

    double closestValue = getDoubleVal(index->nodes[currentIdx]->record, fieldIndex);

    qr->searchString = strdup(query);
    assert(qr->searchString);
    qr->numRecords = numRecords;
    qr->records = records;
    qr->closestValue = closestValue;
    
    return qr;
}


/* Free the given query result. */
void freeQueryResult(struct queryResult *r){
    if(! r){
        return;
    }
    free(r->records);
    free(r->searchString);
    free(r);
}

void freeData(struct data *d){
    if(! d){
        return;
    }
    if(d->building_address){
        free(d->building_address);
    }
    if(d->clue_small_area){
        free(d->clue_small_area);
    }
    if(d->business_address){
        free(d->business_address);
    }
    if(d->trading_name){
        free(d->trading_name);
    }
    if(d->industry_description){
        free(d->industry_description);
    }
    if(d->seating_type){
        free(d->seating_type);
    }
    free(d);
}

/* Free a given dictionary. */
void freeDict(struct dictionary *dict){
    if(! dict){
        return;
    }
    struct dictionaryNode *current = dict->head;
    struct dictionaryNode *next;
    while(current){
        next = current->next;
        freeData(current->record);
        free(current);
        current = next;
    }
    if(dict->indices){
        for(int i = 0; i < NUM_FIELDS; i++){
            if(dict->indices[i]){
                if(dict->indices[i]->nodes){
                    free(dict->indices[i]->nodes);
                }
                free(dict->indices[i]);
            }
        }
        free(dict->indices);
    }
    free(dict);
}

