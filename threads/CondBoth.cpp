#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <string>

using namespace std;

mutex mu;
condition_variable cv;
bool ready = false;
int ctr = 0;

void func() {
	for (int i = 0; i <= 10; i++) {
		static int count = 0;
		count++;
		{
			std::unique_lock<std::mutex> guard(mu);
			cout << "thread : " << count << "     :t1" << endl;
			ctr++;
			if (ctr == 2) {
				ready = true;
				ctr = 0;
				cv.notify_all();
			}
			cv.wait(guard, [] {return !ctr; });
			
		}	
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		

	}
}

void func2() {
	for (int i = 0; i <= 10; i++) {
		static int count2 = 0;
		count2++;
		{
			std::unique_lock<std::mutex> guard(mu);
			cout << "thread : " << count2 << "     :t2 " <<   endl;
			ctr++;
			if (ctr == 2) {
				ready = true;
				ctr = 0;
				cv.notify_all();
			}
			cv.wait(guard, [] {return !ctr; });
		}
	    std::this_thread::sleep_for(std::chrono::milliseconds(10));
		
	}
}



int main()
{
	std::thread t1(func);
	std::thread t2(func2);
	t1.join();
	t2.join();


	cin.get();
	return 0;
}
