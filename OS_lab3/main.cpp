//
//  main.cpp
//  OS_lab3
//
//  Created by MacBook Pro on 29/03/2019.
//  Copyright © 2019 MacBook Pro. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

struct params {
    bool flag;
    int* fd;
};

using namespace std;
void* writing_thread(void* args) {
    params * par = (params *)args;
    
    while (par->flag) {
        string message = " message is " + to_string(random()%20) + "\n";
        write(par->fd[1], message.c_str(), message.length());
        cout << "From writing thread " << endl;
        cout << message;
        sleep(1);
    }
    cout << "Writing thread finished working";
    pthread_exit(0);
}
void* reading_thread(void* args) {
    params * par = (params *)args;
    int flags = fcntl(par->fd[0], F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(par->fd[0], F_SETFL, flags);
    char buf[1024];
    while (par->flag) {
        int r = read(par->fd[0], buf, 1023);
        cout << "From reading thread" << endl;
        if (r > 0) {
            cout << buf;
        }
        else {
            cout << "i have nothing to read" << endl;
            perror("read");
            sleep(2);
            cout << endl;
        }
        
    }
    cout << "Reading thread finished working" << endl;
    pthread_exit(0);
    
}

int main(int argc, char** argv) {
    int descriptors[2];
    pipe(descriptors);
    void* exit_status;
    params param1 = {1, descriptors};
    params param2 = {1,descriptors};
    
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, writing_thread, &param1);
    pthread_create(&t2, NULL, reading_thread, &param2);
    getchar();
    param1.flag = param2.flag = 0;
    
    pthread_join(t1, &exit_status);
    pthread_join(t2, &exit_status);
    close(descriptors[0]);
    close(descriptors[1]);
    printf("%ld\n", *((long*)exit_status));
    printf("%ld\n", *((long*)exit_status));
    
    return 0;
}
