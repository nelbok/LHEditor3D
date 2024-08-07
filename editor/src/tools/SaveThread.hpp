#pragma once

#include "FileThread.hpp"

namespace fse {
class Manager;

class SaveThread : public FileThread {
	Q_OBJECT

public:
	SaveThread(Manager* manager);
	virtual ~SaveThread();

protected:
	virtual void run() override;
};
} // namespace fse
