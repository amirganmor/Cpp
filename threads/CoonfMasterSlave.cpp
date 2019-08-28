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
bool ready = true;

void func() {
	for (int i = 0; i <= 10; i++) {
		static int count = 0;
		{
			std::unique_lock<std::mutex> guard(mu);
			count++;
			cout << "thread : " << count << endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		ready = true;
		cv.notify_all();

	}
}

void func2() {
	for (int i = 0; i <= 10; i++) {
		static int count2 = 0;
		{
			std::unique_lock<std::mutex> guard(mu);
			count2++;
			cout << "thread : " << count2 << endl;
			cv.wait(guard, [] {return ready; });
			ready = false;
		}		
		std::this_thread::sleep_for(std::chrono::milliseconds(500));

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
