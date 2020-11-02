// *** This program can be running in LINUX ***


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

// Mutex - for lock
pthread_mutex_t m_lock;
pthread_mutex_t r_lock;

//MACRO
#define METADATA    "metadata.dat"      // metadata file name

/* Thread input arguments */
typedef struct ThInput{
    char* fname1;           // original file name
    char* fname2;           // new file name
    int blk_num;            // order of block
    int r_size;             // for calculate save size
    int blk_size;           // splitted size
}ThIn;

/* Function Define*/
int store_f(char* fname1, char* fname2, int blk);   // STORE
int read__f(char* fname1, char* fname2);
int list_f();

int main()
{
    char command[255];       // Command
    char fname1[255];       // Original file name of Stored file name
    char fname2[255];       // New file name to store
    int blk = 0;            // # of block to store
    char* st = "store";     // Store command
    char* rd = "read";      // Read command
    char* lt = "list";      // List command
    char* ed = "end";       // Process end command

    pthread_mutex_init(&m_lock, NULL);  // initialize mutex
    pthread_mutex_init(&r_lock, NULL);

    /* Command */
    printf("\n\n-----------------------------------------------------------------------\n");
    printf(" Store the file :\n       >> store (input file name) (store file name) (# of file blocks)\n");
    printf(" Read the file :\n       >> read (stored file name) (output file name)\n");
    printf(" Check the file list :\n       >> list\n");
    printf(" If you want to close the program :\n      >> end\n");
    printf("-----------------------------------------------------------------------\n");

    /* CLIENT */
    while(1){
        /* Get Command */
        printf("\n@ Enter the Command and File name. : >> ");
        scanf("%s", command);
        for(int i =0; i<strlen(command);i++)   // to remove character sensitive for command
            command[i] = tolower(command[i]);

        if(strcmp(command, st) == 0){       // command = store
            scanf("%s", fname1);            // get the original file name
            scanf("%s", fname2);            // get the segmentation file name
            scanf("%d", &blk);              // get # of blocks
            printf("%s, %s, %d\n", fname1, fname2, blk);
            if( (blk < 1) || (blk > 100) )
                printf("# of blocks range : 0 < (blocks) =< 100 \n");
            else
                store_f(fname1, fname2, blk);
        }
        else if (strcmp(command, rd) == 0){ // command = read
            scanf("%s", fname1);            // get the stored file name
            scanf("%s", fname2);            // get the new file name for restore
            read__f(fname1, fname2);
        }
        else if (strcmp(command, lt) == 0){  // command = list
            list_f();
        }
        else if (strcmp(command, ed) == 0){
            printf("\n\n@@ END OF THE CLIENT PROGRAM. @@\n\n * * * * * * GOODBYE! * * * * * *\n\n\n");
            return 0;
        }
        else{
            printf("\n*** Invalid command ***\nTry Again!\n\n");
        }
    }
    
    return 0;
}


/* Read file from Server */
int read__f(char *fname1, char *fname2)
{    
    FILE *mfp; // Metadata file pointer

    mfp = fopen(METADATA, "r");     // open metadata file - reading mode
    if (mfp == NULL)
    {
        printf("\n*** There is no metadata or no file in the server ***\n");
        return 1;
    }

    bool fnd = false;               // STATUS OF Existence of file name in metadata
    char buffer[255];               // for getting a line

    while (fgets(buffer, 255, mfp)) // get a line from metadata before meeting the end of the file
    { // get a line from metadata file
        int wd = 0;         // to count number of words in a line
        int blk = 0;        // # of (file name 1) blocks
        if (strstr(buffer, fname1)) // is there a file name in that line?
        { // find (file name 1) in the line
            char *wptr[4];              // buffer
            wptr[wd] = strtok(buffer, " ");     // cutting the string

            for (int wd = 0; wd < 4; wd++)
            {                                   // 1 : No., 2: file name, 3 : file size, 4 : # of blocks
                wptr[wd] = strtok(NULL, "\t");  // split the line as word

                if (wd == 3)                    // 4th word is '# of blocks'
                    blk = atoi(wptr[wd]);       // # of blocks, change the type from 'char' to 'int'
            }                                   // end of get # of blocks
            fnd = true;                        // if the file exist in medatada, fnd is TRUE

            printf("blk : %d\n", blk);

            char *filename, *ext;         // filename and extention
            filename = strtok(fname1, "."); // filename is before "."
            ext = strtok(NULL, " ");        // extention is after "."
            // Add the data to restore file
            for (int j = 0; j < blk; j++)
            {
                FILE *rfp;              // Reading file pointer
                char *nfname;           // splitted file nmae
                int temp;               // status of fget(reading file)

                nfname = (char *)malloc(sizeof(fname1) + 3);        // Memory allocation
                if (j < 10)
                    sprintf(nfname, "%s_0%d.%s", filename, j, ext); // reading file name with block number
                else
                    sprintf(nfname, "%s_%d.%s", filename, j, ext);

                rfp = fopen(nfname, "r");                           // open reading file_# - read mode
                if (rfp == NULL)
                { // if the file doesn't exist
                    printf("\n*** Server file %d error ****\n", j);
                    return 1;
                }

                FILE *nfp; // restore file pointer ( new file )

                /*** Critical Section START!! ***/
                            pthread_mutex_lock(&r_lock);    // lock

                            nfp = fopen(fname2, "a");   // (create or) addition mode
                            if (nfp == NULL)
                            {
                                printf("\n*** Restore file error ***\n");
                                return 1;
                            }

                            while (EOF != (temp = fgetc(rfp)))
                            {
                                fputc(temp, nfp);   // write data to the new file
                            } // End of write a file

                            pthread_mutex_unlock(&r_lock);  // unlock

                /*** Critical Section End!! ***/
                fclose(rfp);    // release reading file pointer
                fclose(nfp);    // release new file pointer
                free(nfname);
            } // End of write whole file
        }     // End of find (file name 1) and make a restore file
    }         // End of find (file name 1) and make a restore file
    fclose(mfp);    // release metadata file pointer

    if (!fnd)   // if fnd is FALSE ( there isn't file name 1  in metadata.dat)
    { // if there is no file (file name 1) in metadata
        printf("\n*** File name error ***\n     > There is no file : \"%s\"\n\n", fname1);
        return 1;
    }

    
    printf("\nComplete restoring file from \"%s\" to \"%s.\"\n", fname1, fname2);

    return 0;
}


/* Save Thread Function */
void* s_routine(void *arg){
    // Variables
    ThIn *input = (ThIn*)arg;                   // typecast from (void*) to (ThIn*)
    char* fname1 = strdup(input->fname1);       // reading file name
    char* fname2 = strdup(input->fname2);       // storing file name
    int blk_size = input->blk_size;             // splitted file size
    int r_size = input->r_size;                 // remain of block size
    int blk_num = input->blk_num;               // new file's block number
    int save_size = blk_size + r_size;          // storing file size

    FILE *rfp;          // reading file pointer
    FILE *wfp;          // writing file pointer
    void *buffer;       // file buffer
    char *ext;          // file extention
    char *filename;     // file name without file extention
    char *s_file;       // file name with block number

    buffer = malloc(save_size + 1); // splitted size + 1 (NULL)
    if (buffer == NULL)    {        // Memory overflow
        printf("\n\n*** Memory overflow ***\n\n");
        return (void*)1;
    }
    
    /*** Critical Section Start!! ***/
            pthread_mutex_lock(&m_lock);                    // MUTEX LOCK

            rfp = fopen(fname1, "r");                       // open the original file - read mode
            fseek(rfp, (blk_num * blk_size), SEEK_SET);     // go to the location to start reading the file
            memset(buffer, 0, save_size + 1);               // memory initializaton
            fread(buffer, save_size, 1, rfp);               // read file to buffer
            fclose(rfp);                                    // close reading file pointer

            pthread_mutex_unlock(&m_lock);                  // MUTEX UNLOCK
    /*** Critical Section End!! ***/
    

    filename = strtok(fname2, "."); // filename is before "."
    ext = strtok(NULL, " ");        // extention is after "."

    s_file = (char*)malloc(sizeof(fname2)+3);

    if(blk_num<10)
        sprintf(s_file,"%s_0%d.%s", filename, blk_num, ext);
    else
        sprintf(s_file, "%s_%d.%s", filename, blk_num, ext);    
    
    

    wfp = fopen(s_file, "w"); // open(create) file, write mode
    if (wfp == NULL)
    {
        printf("\n*** Writing file %d error ***\n", blk_num);
        return (void *)1;
    }

    fwrite(buffer, save_size, 1, wfp);  // wirte to new file

    fclose(wfp);                        // release writing file pointer
    free(buffer);                       // free the buffer

    printf("\nFile < %s > stored.\n", s_file);    // Finish the thread
    free(s_file);                               // free the string

    return (void*)0;
}


/* Write file to Server */
int store_f(char* fname1, char* fname2, int blk){
    FILE *rfp;                  // reading file pointer

    rfp = fopen(fname1, "r");   // open original file
    if(rfp == NULL){
        printf("\n*** Reading file error ***\n");
        return 1;
    }

    fseek(rfp, 0, SEEK_END);    // pointer : End of the file

    int size = ftell(rfp);      // original file size
    int blk_size = size/blk;    // splitted file size
    int r_size = size%blk;      // remain of splitted size
    fclose(rfp);                // release reading file pointer
    
    pthread_t s_thread[100];        // store threads;
    int th_result;                  // return value of Threads
    ThIn *input[100];               // Thread arguments

    for(int k=0; k<blk; k++){
        // argument block
        input[k] = (ThIn*)malloc(sizeof(ThIn));
        input[k]->fname1 = strdup(fname1);      // original file name
        input[k]->fname2 = strdup(fname2);      // new file name
        input[k]->blk_num   = k;        // # of blocks
        input[k]->blk_size  = blk_size; // splitted size
        if(k == (blk-1))                // if the last block,
            input[k]->r_size = r_size;
        else 
            input[k]->r_size = 0;

        if (pthread_create(&s_thread[k], NULL, s_routine, (void *)input[k]) < 0)
        { // Thread creation
            printf("\n\n*** Thread %d Creation Error ***\n\n", k);
            return 1;
        }
    }

    for (int k = 0; k < blk; k++){
        pthread_join(s_thread[k], (void **)&th_result); // wait for the end of Thread
        free(input[k]);
        if(((int)th_result) != 0){
            printf("\n\n*** Thread %d return error ***\n\n", k);
            return 1;
        }
    }

    FILE *mfp;          // metadata file pointer

    mfp = fopen(METADATA, "a+");      // Open metadata file
    if(mfp == NULL){
        printf("\n*** Metadata file error ***\n");
        return 1;
    }

    char strTemp[255];
    char* pStr;
    int cnt=0;
    while(!feof(mfp)){      // Count lines of metadata file
        pStr = fgets(strTemp, sizeof(strTemp), mfp);
        cnt++;      // when the first time of opening metadata, cnt = 0.
    }
    if(cnt == 0){   // if file doesn't exist             // Only available add lines
        fprintf(mfp, "%-5s\t%-100s\t%15s\t%s\n", "No.","File Name", "File Size", "# of Blocks"); // Header
        cnt ++;     // for line count.
    }

    // Print a new metadata
    if(size<1024)
        fprintf(mfp, "\n%-3d\t%-100s\t%12dB\t%d", cnt, fname2, size, blk);                  // file size unit : B
    else if (size < (1024*1024))
        fprintf(mfp, "\n%-3d\t%-100s\t%11dKB\t%d", cnt, fname2, size/(1024), blk);                 // file size unit : KB
    else
        fprintf(mfp, "\n%-3d\t%-100s\t%11dMB\t%d", cnt, fname2, (size/(1024*1024)), blk);            // file size unit : MB

    fclose(mfp);    // Close metadata file

    printf("\nComplete store the \"%s\" to \"%s\" as %d blocks.\n", fname1, fname2, blk);
    return 0;
}


/* Print the list of the file that in Server */
int list_f(){
    FILE *lfp;      // List file pointer

    lfp = fopen(METADATA, "r");   // Open metadata file, read mode
    if(lfp == NULL){
        printf("\n*** Metadata file error ***\n");
        return 1;
    }

    char strTemp[255];      // line buffer
    char *pStr;             // a line data
    printf("\n");
    for(int i = 0; i<20;i++){
        printf("---------");
    }
    printf("\n");

    while (!feof(lfp))              // before meeting the End of file
    { // Count lines of metadata file
        pStr = fgets(strTemp, sizeof(strTemp), lfp);    // Get a line from metadata file.
        printf("%s\n", pStr);       // Print a line of metadata.
    }

    printf("\n");
    for (int i = 0; i < 20; i++)
    {
        printf("---------");
    }
    printf("\n");

    return 0;
}
