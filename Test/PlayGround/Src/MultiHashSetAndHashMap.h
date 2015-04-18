/* 
 * CX - C++ framework for general purpose developement
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

#pragma once


#include "CX/SparseMultiHashSet.hpp"
#include "CX/SparseMultiHashMap.hpp"
#include "CX/DenseMultiHashSet.hpp"
#include "CX/DenseMultiHashMap.hpp"
#include "CX/Print.hpp"
#include "CX/Mem.hpp"


inline void MultiHashSetAndHashMap_Test1()
{
	{
		CX::SparseMultiHashSet<int>        sset1;
		sset1.SetDeletedKeys(-1);
		sset1.Add(100);
		sset1.Add(200);
		sset1.Add(300);
		CX::Print(stdout, "Contains(100) == true (actual = {1})\n", sset1.Contains<1>(100));

		CX::SparseMultiHashSet<int, int>   sset2;
		sset2.SetDeletedKeys(-1, -1);
		sset2.Add(100, 1000);
		sset2.Add(200, 2000);
		sset2.Add(300, 3000);
		CX::Print(stdout, "Contains<1>(100) == true (actual = {1})\n", sset2.Contains<1>(100));
		CX::Print(stdout, "Contains<2>(1000) == true (actual = {1})\n", sset2.Contains<2>(1000));

		CX::SparseMultiHashSet<int, int, int>   sset3;
		sset3.SetDeletedKeys(-1, -1, -1);
		sset3.Add(100, 1000, 10000);
		sset3.Add(200, 2000, 20000);
		sset3.Add(300, 3000, 30000);
		CX::Print(stdout, "Contains<1>(100) == true (actual = {1})\n", sset3.Contains<1>(100));
		CX::Print(stdout, "Contains<2>(1000) == true (actual = {1})\n", sset3.Contains<2>(1000));
		CX::Print(stdout, "Contains<3>(10000) == true (actual = {1})\n", sset3.Contains<3>(10000));

		CX::SparseMultiHashSet<int, int, int, int>   sset4;
		sset4.SetDeletedKeys(-1, -1, -1, -1);
		sset4.Add(100, 1000, 10000, 100000);
		sset4.Add(200, 2000, 20000, 200000);
		sset4.Add(300, 3000, 30000, 300000);
		CX::Print(stdout, "Contains<1>(100) == true (actual = {1})\n", sset4.Contains<1>(100));
		CX::Print(stdout, "Contains<2>(1000) == true (actual = {1})\n", sset4.Contains<2>(1000));
		CX::Print(stdout, "Contains<3>(10000) == true (actual = {1})\n", sset4.Contains<3>(10000));
		CX::Print(stdout, "Contains<4>(100000) == true (actual = {1})\n", sset4.Contains<4>(100000));

		CX::SparseMultiHashMap<CX::String, int>        smap1;
		smap1.SetDeletedKeys(-1);
		smap1.Add("100", 100);
		smap1.Add("200", 200);
		smap1.Add("300", 300);
		CX::Print(stdout, "Search(100) == 100 (actual = {1})\n", smap1.Search<1>(100)->v);

		CX::SparseMultiHashMap<CX::String, int, int>   smap2;
		smap2.SetDeletedKeys(-1, -1);
		smap2.Add("100", 100, 1000);
		smap2.Add("200", 200, 2000);
		smap2.Add("300", 300, 3000);
		CX::Print(stdout, "Search<1>(100) == 100 (actual = {1})\n", smap2.Search<1>(100)->v);
		CX::Print(stdout, "Search<2>(1000) == 100 (actual = {1})\n", smap2.Search<2>(1000)->v);

		CX::SparseMultiHashMap<CX::String, int, int, int>   smap3;
		smap3.SetDeletedKeys(-1, -1, -1);
		smap3.Add("100", 100, 1000, 10000);
		smap3.Add("200", 200, 2000, 20000);
		smap3.Add("300", 300, 3000, 30000);
		CX::Print(stdout, "Search<1>(100) == 100 (actual = {1})\n", smap3.Search<1>(100)->v);
		CX::Print(stdout, "Search<2>(1000) == 100 (actual = {1})\n", smap3.Search<2>(1000)->v);
		CX::Print(stdout, "Search<3>(10000) == 100 (actual = {1})\n", smap3.Search<3>(10000)->v);

		CX::SparseMultiHashMap<CX::String, int, int, int, int>   smap4;
		smap4.SetDeletedKeys(-1, -1, -1, -1);
		smap4.Add("100", 100, 1000, 10000, 100000);
		smap4.Add("200", 200, 2000, 20000, 200000);
		smap4.Add("300", 300, 3000, 30000, 300000);
		CX::Print(stdout, "Search<1>(100) == 100 (actual = {1})\n", smap4.Search<1>(100)->v);
		CX::Print(stdout, "Search<2>(1000) == 100 (actual = {1})\n", smap4.Search<2>(1000)->v);
		CX::Print(stdout, "Search<3>(10000) == 100 (actual = {1})\n", smap4.Search<3>(10000)->v);
		CX::Print(stdout, "Search<4>(100000) == 100 (actual = {1})\n", smap4.Search<4>(100000)->v);

		CX::DenseMultiHashSet<int>        dset1;
		dset1.SetDeletedKeys(-1);
		dset1.SetEmptyKeys(-2);
		dset1.Add(100);
		dset1.Add(200);
		dset1.Add(300);
		CX::Print(stdout, "Contains(100) == true (actual = {1})\n", dset1.Contains<1>(100));

		CX::DenseMultiHashSet<int, int>   dset2;
		dset2.SetDeletedKeys(-1, -1);
		dset2.SetEmptyKeys(-2, -2);
		dset2.Add(100, 1000);
		dset2.Add(200, 2000);
		dset2.Add(300, 3000);
		CX::Print(stdout, "Contains<1>(100) == true (actual = {1})\n", dset2.Contains<1>(100));
		CX::Print(stdout, "Contains<2>(1000) == true (actual = {1})\n", dset2.Contains<2>(1000));

		CX::DenseMultiHashSet<int, int, int>   dset3;
		dset3.SetDeletedKeys(-1, -1, -1);
		dset3.SetEmptyKeys(-2, -2, -2);
		dset3.Add(100, 1000, 10000);
		dset3.Add(200, 2000, 20000);
		dset3.Add(300, 3000, 30000);
		CX::Print(stdout, "Contains<1>(100) == true (actual = {1})\n", dset3.Contains<1>(100));
		CX::Print(stdout, "Contains<2>(1000) == true (actual = {1})\n", dset3.Contains<2>(1000));
		CX::Print(stdout, "Contains<3>(10000) == true (actual = {1})\n", dset3.Contains<3>(10000));

		CX::DenseMultiHashSet<int, int, int, int>   dset4;
		dset4.SetDeletedKeys(-1, -1, -1, -1);
		dset4.SetEmptyKeys(-2, -2, -2, -2);
		dset4.Add(100, 1000, 10000, 100000);
		dset4.Add(200, 2000, 20000, 200000);
		dset4.Add(300, 3000, 30000, 300000);
		CX::Print(stdout, "Contains<1>(100) == true (actual = {1})\n", dset4.Contains<1>(100));
		CX::Print(stdout, "Contains<2>(1000) == true (actual = {1})\n", dset4.Contains<2>(1000));
		CX::Print(stdout, "Contains<3>(10000) == true (actual = {1})\n", dset4.Contains<3>(10000));
		CX::Print(stdout, "Contains<4>(100000) == true (actual = {1})\n", dset4.Contains<4>(100000));

		CX::DenseMultiHashMap<CX::String, int>        dmap1;
		dmap1.SetDeletedKeys(-1);
		dmap1.SetEmptyKeys(-2);
		dmap1.Add("100", 100);
		dmap1.Add("200", 200);
		dmap1.Add("300", 300);
		CX::Print(stdout, "Search(100) == 100 (actual = {1})\n", dmap1.Search<1>(100)->v);

		CX::DenseMultiHashMap<CX::String, int, int>   dmap2;
		dmap2.SetDeletedKeys(-1, -1);
		dmap2.SetEmptyKeys(-2, -2);
		dmap2.Add("100", 100, 1000);
		dmap2.Add("200", 200, 2000);
		dmap2.Add("300", 300, 3000);
		CX::Print(stdout, "Search<1>(100) == 100 (actual = {1})\n", dmap2.Search<1>(100)->v);
		CX::Print(stdout, "Search<2>(1000) == 100 (actual = {1})\n", dmap2.Search<2>(1000)->v);

		CX::DenseMultiHashMap<CX::String, int, int, int>   dmap3;
		dmap3.SetDeletedKeys(-1, -1, -1);
		dmap3.SetEmptyKeys(-2, -2, -2);
		dmap3.Add("100", 100, 1000, 10000);
		dmap3.Add("200", 200, 2000, 20000);
		dmap3.Add("300", 300, 3000, 30000);
		CX::Print(stdout, "Search<1>(100) == 100 (actual = {1})\n", dmap3.Search<1>(100)->v);
		CX::Print(stdout, "Search<2>(1000) == 100 (actual = {1})\n", dmap3.Search<2>(1000)->v);
		CX::Print(stdout, "Search<3>(10000) == 100 (actual = {1})\n", dmap3.Search<3>(10000)->v);

		CX::DenseMultiHashMap<CX::String, int, int, int, int>   dmap4;
		dmap4.SetDeletedKeys(-1, -1, -1, -1);
		dmap4.SetEmptyKeys(-2, -2, -2, -2);
		dmap4.Add("100", 100, 1000, 10000, 100000);
		dmap4.Add("200", 200, 2000, 20000, 200000);
		dmap4.Add("300", 300, 3000, 30000, 300000);
		CX::Print(stdout, "Search<1>(100) == 100 (actual = {1})\n", dmap4.Search<1>(100)->v);
		CX::Print(stdout, "Search<2>(1000) == 100 (actual = {1})\n", dmap4.Search<2>(1000)->v);
		CX::Print(stdout, "Search<3>(10000) == 100 (actual = {1})\n", dmap4.Search<3>(10000)->v);
		CX::Print(stdout, "Search<4>(100000) == 100 (actual = {1})\n", dmap4.Search<4>(100000)->v);
	}

	CX::Mem::AllocsVector vectorAllocs;

	CX::Mem::GetCurrentAllocs(vectorAllocs);
	CX::Mem::PrintAllocs(stdout, vectorAllocs);
}

