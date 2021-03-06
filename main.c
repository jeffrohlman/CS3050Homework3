/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Jeffrey
 *
 * Created on October 16, 2017, 1:50 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "input_error.h"
#include "vector.h"
#include <ctype.h>
#include "queue.h"

void parse_getline(FILE*, struct vector adj[], int max);
void parseline(char *, int *, int *, int max);
void bfs(struct vector adj[], int dist[], int max);


int main(int argc, char** argv) {
    if(argc != 3)
        exit(INCORRECT_NUMBER_OF_COMMAND_LINE_ARGUMENTS);
    
    FILE* fptr = fopen(*(argv + 1), "r");
    if(!fptr)
        exit(INPUT_FILE_FAILED_TO_OPEN);
    
    
    if(fgetc(fptr) == EOF)
        exit(PARSING_ERROR_EMPTY_INPUT_FILE);
    fseek(fptr, 0, SEEK_SET);
    
    char *line = NULL;
    size_t nbytes = 0;
    getline(&line, &nbytes, fptr);
    
    int i = 0;
    int num = 0;
    while(line[i] != '\n'){
        if(isdigit(line[i]) == 0)
            exit(PARSING_ERROR_INVALID_FORMAT);
        num *= 10;
        num += line[i] - '0';
        i++;
    }
    
    struct vector *adjList = (struct vector *)malloc(sizeof(struct vector) * num);
    int *dist = (int *)malloc(sizeof(int) * num);
    for(i = 0; i < num; i++){
        init_vector(&adjList[i]);
        dist[i] = -1;
    }
    parse_getline(fptr, adjList, num);
    
    bfs(adjList, dist, num);
    
    
    FILE* ofptr = fopen(*(argv + 2), "w");
    if(!ofptr)
        exit(OUTPUT_FILE_FAILED_TO_OPEN);
    
    for(i = 0; i < num; i++)
        fprintf(ofptr, "%d\n", dist[i]);
    
    if((fclose(fptr)) != 0)
        exit(INPUT_FILE_FAILED_TO_CLOSE);
    
    if((fclose(ofptr)) != 0)
        exit(OUTPUT_FILE_FAILED_TO_CLOSE);
    
    free(line);
    for(i = 0; i < num; i++)
        free_vector(&adjList[i]);
    free(dist);
    
    return (EXIT_SUCCESS);
}

//Function that scans file and pulls each line one at a time
//Send individual lines to parseline to check for non digit characters
//Adds to vector once determined to be an integer
void parse_getline(FILE* fp, struct vector adj[], int max) {
	char* line = NULL;
	size_t nbytes = 0;
        int linelen=0;
        int v1 = 0;
        int v2 = 0;
        
	while ((linelen=getline(&line, &nbytes, fp)) != -1) {
		line[linelen-1] = '\0'; 
		parseline(line, &v1, &v2, max);
                insert_element_vector(&adj[v1 - 1], v2);
                insert_element_vector(&adj[v2 - 1], v1);   
	}

	free(line);
}

//Function to take in single line and parse for non digit characters
//If the function encounters a non digit character the program is exited
void parseline(char *line, int *v1, int *v2, int max) {
	char c;
        int i=1;
        *v1 = 0;
        *v2 = 0;
        if(line[0] != '(')
            exit(PARSING_ERROR_INVALID_FORMAT);

	while ((c = line[i++]) != ',') {
		if (!isdigit(c))
                    exit(PARSING_ERROR_INVALID_FORMAT);
                *v1 *= 10;
                *v1 += c - '0';
	}
        if(*v1 > max)
            exit(INTEGER_IS_NOT_A_VERTEX);
        while ((c = line[i++]) != ')') {
		if (!isdigit(c))
                    exit(PARSING_ERROR_INVALID_FORMAT);
                *v2 *= 10;
                *v2 += c - '0';
	}
        if(*v2 > max)
            exit(INTEGER_IS_NOT_A_VERTEX);
        if(line[i] != '\0')
            exit(PARSING_ERROR_INVALID_FORMAT);
}

void bfs(struct vector adj[], int dist[], int max){
    dist[0] = 0;
    int v1 = 0;
    int v2 = 0;
    int i = 0;
    struct Queue* q = createQueue(max);
    enqueue(q, 1);
    while(!isEmpty(q)){
        v1 = dequeue(q);
        for(i = 0; i < vector_size(&adj[v1 - 1]); i++){
            v2 = access_element_vector(&adj[v1 - 1], i);
            if(dist[v2 - 1] == -1){
                dist[v2 - 1] = dist[v1 - 1] + 1;
                enqueue(q, v2);
            }
        }
    }
}