/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014-2015 draede - draede [at] outlook [dot] com
 *
 * Released under the MIT License.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */ 

#include "CX/KVDB/MemDatabase.hpp"
#include "CX/IO/FileInputStream.hpp"
#include "CX/IO/FileOutputStream.hpp"
#include "CX/IO/LocalFileSysHelper.hpp"
#include "CX/Hash/CRC32Hash.hpp"
#include "CX/Print.hpp"
#include "MemDBSerialize.hpp"


namespace CX
{

namespace KVDB
{

const MemDatabaseConfig        MemDatabase::DEFAULT_DB_CONFIG;
const MemTableConfig           MemDatabase::DEFAULT_TABLE_CONFIG;
const MemDatabase::MagicType   MemDatabase::MAGIC = 0x302E3142444D5843; //CXMDB1.0

MemDatabase::MemDatabase()
{
	m_mapTables.set_deleted_key("{F1D78E19-32ED-4353-89E8-335913009A37}");
}

MemDatabase::~MemDatabase()
{
	Close();
}

void MemDatabase::FreeMem()
{
	for (TablesMap::iterator iter = m_mapTables.begin(); iter != m_mapTables.end(); ++iter)
	{
		delete iter->second;
	}
	m_mapTables.clear();
}

Status MemDatabase::Open(const Char *szPath, const IDatabaseConfig &config)
{
	Close();

	config;
	//const MemDatabaseConfig &conf = dynamic_cast<const MemDatabaseConfig &>(config);

	m_sPath = NULL == szPath ? "" : szPath;
	Load();

	return Status();
}

Status MemDatabase::Close()
{
	Save();
	m_sPath.clear();
	FreeMem();

	return Status();
}

Status MemDatabase::OpenTable(ITable **ppTable, const ITableConfig &config, const Char *szName)
{
	TablesMap::iterator iter;

	config;

	if (m_mapTables.end() != (iter = m_mapTables.find(szName)))
	{
		*ppTable = iter->second;

		return Status();
	}

	MemTable *pTable = new MemTable(this, szName);

	m_mapTables[szName] = pTable;

	*ppTable = pTable;

	return Status();
}

Status MemDatabase::CloseTable(ITable *pTable)
{
	MemTable *pRealTable = dynamic_cast<MemTable *>(pTable);

	if (NULL == pRealTable)
	{
		return Status(Status_InvalidArg, "Not a valid table");
	}

	TablesMap::iterator iter;

	if (m_mapTables.end() == (iter = m_mapTables.find(pRealTable->GetName())))
	{
		return Status(Status_NotFound, "Table not found");
	}

	return Status();
}

Status MemDatabase::DropTable(const Char *szName)
{
	TablesMap::iterator iter;

	if (m_mapTables.end() == (iter = m_mapTables.find(szName)))
	{
		return Status(Status_NotFound, "Table not found");
	}
	delete iter->second;
	m_mapTables.erase(iter);

	return Status();
}

Status MemDatabase::BeginTransaction()
{
	return Status();
}

Status MemDatabase::EndTransaction(bool bAbort/* = false*/)
{
	bAbort;

	return Status();
}

Status MemDatabase::Commit()
{
	return Status();
}

bool MemDatabase::IsOK() const
{
	return true;
}

const Char *MemDatabase::GetPath() const
{
	return m_sPath.c_str();
}

bool MemDatabase::IsReadOnly() const
{
	return false;
}

Status MemDatabase::Save()
{
	if (m_sPath.empty())
	{
		return Status();
	}

	return Save(m_sPath.c_str());
}


Status MemDatabase::Save(const Char *szPath)
{
	String sTmpPath;
	Status status;

	Print(&sTmpPath, "{1}.tmp", szPath);

	{
		IO::FileOutputStream fos(sTmpPath.c_str());

		if (!fos.IsOK())
		{
			return Status(Status_CreateFailed, "Failed to create file '{2}'", sTmpPath);
		}
		if ((status = Save(&fos)).IsNOK())
		{
			return status;
		}
	}

	return IO::LocalFileSysHelper::Get().RenameFile(sTmpPath.c_str(), szPath);
}

Status MemDatabase::Save(IO::IOutputStream *pOutputStream)
{
	Hash::CRC32Hash   hash;
	Byte              hashdata[Hash::CRC32Hash::SIZE];
	Status            status;

	hash.Init();
	if ((status  = MemDBSerialize::WriteData(pOutputStream, &MAGIC, sizeof(MAGIC), &hash)).IsNOK())
	{
		return status;
	}
	if ((status = MemDBSerialize::WriteCount(pOutputStream, m_mapTables.size(), &hash)).IsNOK())
	{
		return status;
	}
	for (TablesMap::iterator iter = m_mapTables.begin(); iter != m_mapTables.end(); ++iter)
	{
		if ((status = iter->second->Save(pOutputStream, &hash)).IsNOK())
		{
			return status;
		}
	}
	hash.Done(hashdata);
	hash.Init();
	if ((status = MemDBSerialize::WriteData(pOutputStream, hashdata, sizeof(hashdata), &hash)).IsNOK())
	{
		return status;
	}

	return Status();
}

Status MemDatabase::Load()
{
	return Load(m_sPath.c_str());
}

Status MemDatabase::Load(const Char *szPath)
{
	IO::FileInputStream fis(szPath);

	if (!fis.IsOK())
	{
		return Status(Status_CreateFailed, "Failed to open file '{2}'", szPath);
	}

	return Load(&fis);
}

Status MemDatabase::Load(IO::IInputStream *pInputStream)
{
	Hash::CRC32Hash   hash;
	MagicType         nMagic;
	Size              cTablesCount;
	Byte              hashdata[Hash::CRC32Hash::SIZE];
	Byte              filehashdata[Hash::CRC32Hash::SIZE];
	Status            status;

	FreeMem();
	hash.Init();
	if ((status = MemDBSerialize::ReadData(pInputStream, &nMagic, sizeof(nMagic), &hash)).IsNOK())
	{
		return status;
	}
	if (MAGIC != nMagic)
	{
		return Status(Status_ReadFailed, "Invalid magic number");
	}
	if ((status = MemDBSerialize::ReadCount(pInputStream, cTablesCount, &hash)).IsNOK())
	{
		return status;
	}
	if (MAX_TABLES_COUNT < cTablesCount)
	{
		return Status(Status_ReadFailed, "To many tables (max is {1}, {2} found)", MAX_TABLES_COUNT, cTablesCount);
	}
	for (Size i = 0; i < cTablesCount; i++)
	{
		MemTable *pTable = new MemTable(this, "");

		if ((status = pTable->Load(pInputStream, &hash)).IsNOK())
		{
			delete pTable;

			return status;
		}
		m_mapTables[pTable->GetName()] = pTable;
	}
	hash.Done(hashdata);
	hash.Init();
	if ((status = MemDBSerialize::ReadData(pInputStream, filehashdata, sizeof(filehashdata), &hash)).IsNOK())
	{
		return status;
	}
	if (0 != memcpy(hashdata, filehashdata, sizeof(filehashdata)))
	{
		return Status(Status_ReadFailed, "Invalid checksum");
	}

	return Status();
}

}//namespace KVDB

}//namespace CX

