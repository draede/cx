/* 
 * CX - C++ framework for general purpose development
 *
 * https://github.com/draede/cx
 * 
 * Copyright (C) 2014 - 2021 draede - draede [at] outlook [dot] com
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

#pragma once


#include "CX/DenseHashMap.hpp"
#include "CX/Vector.hpp"


namespace CX
{

template <typename K1 = EmptyType, typename K2 = EmptyType, typename K3 = EmptyType, typename K4 = EmptyType>
class DenseMultiHashSet;

template <typename K1>
class DenseMultiHashSet<K1> : public IObject
{
public:

	typedef struct _Data
	{
		K1 k1;
	}Data;

	template <Size INDEX>
	struct KeyType;

	template <>
	struct KeyType<1>
	{
		typedef K1   Type;
	};

	template <Size INDEX>
	struct MapType
	{
		typedef typename DenseHashMap<typename KeyType<INDEX>::Type, Data *>::Type   Type;
	};

	DenseMultiHashSet()
	{
	}

	~DenseMultiHashSet()
	{
		Clear();
	}

	template <Size INDEX>
	typename const MapType<INDEX>::Type &Get() const;

	template <>
	typename const MapType<1>::Type &Get<1>() const
	{
		return m_map1;
	}

	void SetDeletedKeys(const K1 &k1)
	{
		m_map1.set_deleted_key(k1);
	}

	void SetEmptyKeys(const K1 &k1)
	{
		m_map1.set_empty_key(k1);
	}

	Size GetCount() const
	{
		return m_vectorData.size();
	}

	bool IsEmpty() const
	{
		return (0 == GetCount());
	}

	void Add(const K1 &k1)
	{
		DataVector::iterator iter;
		Data                 *pData = new (std::nothrow) Data();

		pData->k1      = k1;
		m_vectorData.push_back(pData);
		GetEx<1>()[k1] = pData;
	}

	template <Size INDEX>
	const Data *Search(const typename KeyType<INDEX>::Type &p) const
	{
		typename MapType<INDEX>::Type::const_iterator iter = Get<INDEX>().find(p);

		if (Get<INDEX>().end() == iter)
		{
			return NULL;
		}

		return iter->second;
	}

	template <Size INDEX>
	bool Contains(const typename KeyType<INDEX>::Type &p) const
	{
		return (NULL != Search<INDEX>(p));
	}

	void Clear()
	{
		GetEx<1>().clear();
		for (DataVector::iterator iter = m_vectorData.begin(); iter != m_vectorData.end(); ++iter)
		{
			delete *iter;
		}
		m_vectorData.clear();
	}

private:

	typedef typename Vector<Data *>::Type   DataVector;

	template <Size INDEX>
	typename MapType<INDEX>::Type &GetEx();

	template <>
	typename MapType<1>::Type &GetEx<1>()
	{
		return m_map1;
	}

	DataVector                  m_vectorData;
	typename MapType<1>::Type   m_map1;

};

template <typename K1, typename K2>
class DenseMultiHashSet<K1, K2> : public IObject
{
public:

	typedef struct _Data
	{
		K1 k1;
		K2 k2;
	}Data;

	template <Size INDEX>
	struct KeyType;

	template <>
	struct KeyType<1>
	{
		typedef K1   Type;
	};

	template <>
	struct KeyType<2>
	{
		typedef K2   Type;
	};
	
	template <Size INDEX>
	struct MapType
	{
		typedef typename DenseHashMap<typename KeyType<INDEX>::Type, Data *>::Type   Type;
	};

	DenseMultiHashSet()
	{
	}

	~DenseMultiHashSet()
	{
		Clear();
	}

	template <Size INDEX>
	typename const MapType<INDEX>::Type &Get() const;

	template <>
	typename const MapType<1>::Type &Get<1>() const
	{
		return m_map1;
	}

	template <>
	typename const MapType<2>::Type &Get<2>() const
	{
		return m_map2;
	}

	void SetDeletedKeys(const K1 &k1, const K2 &k2)
	{
		m_map1.set_deleted_key(k1);
		m_map2.set_deleted_key(k2);
	}

	void SetEmptyKeys(const K1 &k1, const K2 &k2)
	{
		m_map1.set_empty_key(k1);
		m_map2.set_empty_key(k2);
	}

	Size GetCount() const
	{
		return m_vectorData.size();
	}

	bool IsEmpty() const
	{
		return (0 == GetCount());
	}

	void Add(const K1 &k1, const K2 &k2)
	{
		DataVector::iterator iter;
		Data                 *pData = new (std::nothrow) Data();

		pData->k1      = k1;
		pData->k2      = k2;
		m_vectorData.push_back(pData);
		GetEx<1>()[k1] = pData;
		GetEx<2>()[k2] = pData;
	}

	template <Size INDEX>
	const Data *Search(const typename KeyType<INDEX>::Type &p) const
	{
		typename MapType<INDEX>::Type::const_iterator iter = Get<INDEX>().find(p);

		if (Get<INDEX>().end() == iter)
		{
			return NULL;
		}

		return iter->second;
	}

	template <Size INDEX>
	bool Contains(const typename KeyType<INDEX>::Type &p) const
	{
		return (NULL != Search<INDEX>(p));
	}

	void Clear()
	{
		GetEx<1>().clear();
		for (DataVector::iterator iter = m_vectorData.begin(); iter != m_vectorData.end(); ++iter)
		{
			delete *iter;
		}
		m_vectorData.clear();
	}

private:

	typedef typename Vector<Data *>::Type   DataVector;

	template <Size INDEX>
	typename MapType<INDEX>::Type &GetEx();

	template <>
	typename MapType<1>::Type &GetEx<1>()
	{
		return m_map1;
	}
	
	template <>
	typename MapType<2>::Type &GetEx<2>()
	{
		return m_map2;
	}

	DataVector                  m_vectorData;
	typename MapType<1>::Type   m_map1;
	typename MapType<2>::Type   m_map2;

};

template <typename K1, typename K2, typename K3>
class DenseMultiHashSet<K1, K2, K3> : public IObject
{
public:

	typedef struct _Data
	{
		K1 k1;
		K2 k2;
		K3 k3;
	}Data;

	template <Size INDEX>
	struct KeyType;

	template <>
	struct KeyType<1>
	{
		typedef K1   Type;
	};

	template <>
	struct KeyType<2>
	{
		typedef K2   Type;
	};

	template <>
	struct KeyType<3>
	{
		typedef K3   Type;
	};
	
	template <Size INDEX>
	struct MapType
	{
		typedef typename DenseHashMap<typename KeyType<INDEX>::Type, Data *>::Type   Type;
	};

	DenseMultiHashSet()
	{
	}

	~DenseMultiHashSet()
	{
		Clear();
	}

	template <Size INDEX>
	typename const MapType<INDEX>::Type &Get() const;

	template <>
	typename const MapType<1>::Type &Get<1>() const
	{
		return m_map1;
	}

	template <>
	typename const MapType<2>::Type &Get<2>() const
	{
		return m_map2;
	}

	template <>
	typename const MapType<3>::Type &Get<3>() const
	{
		return m_map3;
	}

	void SetDeletedKeys(const K1 &k1, const K2 &k2, const K3 &k3)
	{
		m_map1.set_deleted_key(k1);
		m_map2.set_deleted_key(k2);
		m_map3.set_deleted_key(k3);
	}

	void SetEmptyKeys(const K1 &k1, const K2 &k2, const K3 &k3)
	{
		m_map1.set_empty_key(k1);
		m_map2.set_empty_key(k2);
		m_map3.set_empty_key(k3);
	}

	Size GetCount() const
	{
		return m_vectorData.size();
	}

	bool IsEmpty() const
	{
		return (0 == GetCount());
	}

	void Add(const K1 &k1, const K2 &k2, const K3 &k3)
	{
		DataVector::iterator iter;
		Data                 *pData = new (std::nothrow) Data();

		pData->k1      = k1;
		pData->k2      = k2;
		pData->k3      = k3;
		m_vectorData.push_back(pData);
		GetEx<1>()[k1] = pData;
		GetEx<2>()[k2] = pData;
		GetEx<3>()[k3] = pData;
	}

	template <Size INDEX>
	const Data *Search(const typename KeyType<INDEX>::Type &p) const
	{
		typename MapType<INDEX>::Type::const_iterator iter = Get<INDEX>().find(p);

		if (Get<INDEX>().end() == iter)
		{
			return NULL;
		}

		return iter->second;
	}

	template <Size INDEX>
	bool Contains(const typename KeyType<INDEX>::Type &p) const
	{
		return (NULL != Search<INDEX>(p));
	}

	void Clear()
	{
		GetEx<1>().clear();
		for (DataVector::iterator iter = m_vectorData.begin(); iter != m_vectorData.end(); ++iter)
		{
			delete *iter;
		}
		m_vectorData.clear();
	}

private:

	typedef typename Vector<Data *>::Type   DataVector;

	template <Size INDEX>
	typename MapType<INDEX>::Type &GetEx();

	template <>
	typename MapType<1>::Type &GetEx<1>()
	{
		return m_map1;
	}
	
	template <>
	typename MapType<2>::Type &GetEx<2>()
	{
		return m_map2;
	}

	template <>
	typename MapType<3>::Type &GetEx<3>()
	{
		return m_map3;
	}

	DataVector                  m_vectorData;
	typename MapType<1>::Type   m_map1;
	typename MapType<2>::Type   m_map2;
	typename MapType<3>::Type   m_map3;

};

template <typename K1, typename K2, typename K3, typename K4>
class DenseMultiHashSet : public IObject
{
public:

	typedef struct _Data
	{
		K1 k1;
		K2 k2;
		K3 k3;
		K4 k4;
	}Data;

	template <Size INDEX>
	struct KeyType;

	template <>
	struct KeyType<1>
	{
		typedef K1   Type;
	};

	template <>
	struct KeyType<2>
	{
		typedef K2   Type;
	};

	template <>
	struct KeyType<3>
	{
		typedef K3   Type;
	};
	
	template <>
	struct KeyType<4>
	{
		typedef K4   Type;
	};
	
	template <Size INDEX>
	struct MapType
	{
		typedef typename DenseHashMap<typename KeyType<INDEX>::Type, Data *>::Type   Type;
	};

	DenseMultiHashSet()
	{
	}

	~DenseMultiHashSet()
	{
		Clear();
	}

	template <Size INDEX>
	typename const MapType<INDEX>::Type &Get() const;

	template <>
	typename const MapType<1>::Type &Get<1>() const
	{
		return m_map1;
	}

	template <>
	typename const MapType<2>::Type &Get<2>() const
	{
		return m_map2;
	}

	template <>
	typename const MapType<3>::Type &Get<3>() const
	{
		return m_map3;
	}

	template <>
	typename const MapType<4>::Type &Get<4>() const
	{
		return m_map4;
	}

	void SetDeletedKeys(const K1 &k1, const K2 &k2, const K3 &k3, const K4 &k4)
	{
		m_map1.set_deleted_key(k1);
		m_map2.set_deleted_key(k2);
		m_map3.set_deleted_key(k3);
		m_map4.set_deleted_key(k4);
	}

	void SetEmptyKeys(const K1 &k1, const K2 &k2, const K3 &k3, const K4 &k4)
	{
		m_map1.set_empty_key(k1);
		m_map2.set_empty_key(k2);
		m_map3.set_empty_key(k3);
		m_map4.set_empty_key(k4);
	}

	Size GetCount() const
	{
		return m_vectorData.size();
	}

	bool IsEmpty() const
	{
		return (0 == GetCount());
	}

	void Add(const K1 &k1, const K2 &k2, const K3 &k3, const K4 &k4)
	{
		DataVector::iterator iter;
		Data                 *pData = new (std::nothrow) Data();

		pData->k1      = k1;
		pData->k2      = k2;
		pData->k3      = k3;
		pData->k4      = k4;
		m_vectorData.push_back(pData);
		GetEx<1>()[k1] = pData;
		GetEx<2>()[k2] = pData;
		GetEx<3>()[k3] = pData;
		GetEx<4>()[k4] = pData;
	}

	template <Size INDEX>
	const Data *Search(const typename KeyType<INDEX>::Type &p) const
	{
		typename MapType<INDEX>::Type::const_iterator iter = Get<INDEX>().find(p);

		if (Get<INDEX>().end() == iter)
		{
			return NULL;
		}

		return iter->second;
	}

	template <Size INDEX>
	bool Contains(const typename KeyType<INDEX>::Type &p) const
	{
		return (NULL != Search<INDEX>(p));
	}

	void Clear()
	{
		GetEx<1>().clear();
		for (DataVector::iterator iter = m_vectorData.begin(); iter != m_vectorData.end(); ++iter)
		{
			delete *iter;
		}
		m_vectorData.clear();
	}

private:

	typedef typename Vector<Data *>::Type   DataVector;

	template <Size INDEX>
	typename MapType<INDEX>::Type &GetEx();

	template <>
	typename MapType<1>::Type &GetEx<1>()
	{
		return m_map1;
	}
	
	template <>
	typename MapType<2>::Type &GetEx<2>()
	{
		return m_map2;
	}

	template <>
	typename MapType<3>::Type &GetEx<3>()
	{
		return m_map3;
	}

	template <>
	typename MapType<4>::Type &GetEx<4>()
	{
		return m_map4;
	}

	DataVector                  m_vectorData;
	typename MapType<1>::Type   m_map1;
	typename MapType<2>::Type   m_map2;
	typename MapType<3>::Type   m_map3;
	typename MapType<4>::Type   m_map4;

};

}//namespace CX

