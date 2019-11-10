
#pragma once


#include "CX/CLI/ICommand.hpp"


//fx.exe help


namespace FX
{

class HelpCommand : public CX::CLI::ICommand
{
public:

	static const CX::Char NAME[];
	static const CX::Char DESC[];

	HelpCommand();

	~HelpCommand();

	static CX::CLI::ICommand *Create();

	virtual const CX::Char *GetName();

	virtual const CX::Char *GetDescription();

	virtual CX::Status Init(int argc, const CX::Char *argv[]);

	virtual CX::Status Run();

	virtual CX::Status Uninit();

	virtual void Destroy();

private:

	static CX::Status   REGISTRATION_STATUS;

};

}//namespace FX
