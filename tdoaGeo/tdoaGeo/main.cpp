#include <iostream>
#include <string>
#include <fstream>
#include "json.h"
#include "tdoa.h"
#include "graphics.h"

std::mutex cout_mtx;

SafeQueue<tdoaResult *> results;

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cout << "no configuration specified" << std::endl;
		return 0;
	}
	tdoa tdoa(&results);

	std::string file(argv[1]);
	std::ifstream stream(file);
	Json::Reader reader;
	Json::Value root;

	if (stream.is_open() && reader.parse(stream, root, false))
	{
		tdoa.setParams(root);
	}
	else
	{
		std::cout << "configuration failed" << std::endl;
		return 0;
	}
	//Run tdoa in a different thread so that we can reconfigure it.
	std::thread t(&tdoa::run, &tdoa);
	bool windowOpen = true;
	graphics plot(800, 800);
	while(windowOpen)
	{
		//Make sure we don't lock up and fail to process windows events
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		//results.push(NULL);
		//Pop a pointer to IQ data from a node.
		//We will be blocked waiting for data to appear.
		tdoaResult *result = results.front();
		windowOpen = plot.plotResult(result);
		//windowOpen = plot.drawSignal(result->axes);

		if (result != NULL)
		{
			//std::cout << "popping result " << result->target.timeStamp << std::endl;
			delete result;
		}
		results.pop();
	}

	tdoa.stop();
	t.join();

    return 0;
}

