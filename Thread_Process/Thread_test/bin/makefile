#头文件目录
INCLUDEDIR = -I/root/Thread_Process/Thread_test/include
#普通库文件目录
LIBDIR = /root/Thread_Process/Thread_test/src
#可执行文件目录
BINDIR = /root/Thread_Process/Thread_test/bin
#编译选项
CFLAGS = -lpthread -g

#指定要编译的文件
all:pthread_race pthread_race2 pthread_term pthread_clean pthread_detach \
	account_test \
	lock_type \
	rwlock_feature \
	pthread_cal \
	pthread_cal2 \
	reader_writer \
	sem_test

pthread_race:$(LIBDIR)/pthread_race.cpp
	g++ -o pthread_race $(LIBDIR)/pthread_race.cpp $(CFLAGS)

pthread_race2:$(LIBDIR)/pthread_race2.cpp
	g++ -o pthread_race2 $(LIBDIR)/pthread_race2.cpp $(CFLAGS)

pthread_term:$(LIBDIR)/pthread_term.cpp
	g++ -o pthread_term $(LIBDIR)/pthread_term.cpp $(CFLAGS)

pthread_clean:$(LIBDIR)/pthread_clean.cpp
	g++ -o pthread_clean $(LIBDIR)/pthread_clean.cpp $(CFLAGS)

pthread_detach:$(LIBDIR)/pthread_detach.cpp
	g++ -o pthread_detach $(LIBDIR)/pthread_detach.cpp $(CFLAGS)

account_test:$(LIBDIR)/account_test.cpp
	g++ -o account_test $(INCLUDEDIR) $(LIBDIR)/account_test.cpp $(LIBDIR)/account.cpp $(CFLAGS)

lock_type:$(LIBDIR)/lock_type.cpp
	g++ -o lock_type $(LIBDIR)/lock_type.cpp $(CFLAGS)

rwlock_feature:$(LIBDIR)/rwlock_feature.cpp
	g++ -o rwlock_feature $(LIBDIR)/rwlock_feature.cpp $(CFLAGS)

pthread_cal:$(LIBDIR)/pthread_cal.cpp
	g++ -o pthread_cal $(LIBDIR)/pthread_cal.cpp $(CFLAGS)
	
pthread_cal2:$(LIBDIR)/pthread_cal2.cpp
	g++ -o pthread_cal2 $(LIBDIR)/pthread_cal2.cpp $(CFLAGS)

reader_writer:$(LIBDIR)/reader_writer.cpp
	g++ -o reader_writer $(LIBDIR)/reader_writer.cpp $(CFLAGS)

sem_test:$(LIBDIR)/sem_test.cpp
	g++ -o sem_test $(LIBDIR)/sem_test.cpp $(CFLAGS)


# clean 用于清理编译结果文件，仅在make clean才会执行
clean:
	rm -f pthread_race pthread_race2 pthread_term \
	pthread_clean pthread_detach account_test lock_type\
	rwlock_feature \
	pthread_cal \
	pthread_cal2 \
	sem_test


