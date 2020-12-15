// MemAllocate.cpp: implementation of the CMemAllocate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Memory.h"
#include "MemAllocate.h"

#include "..\OSgraphics\Rectangle.h"
#include <iostream>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMemAllocate::CMemAllocate()
{	
	//���ó�ʼ״̬
	m_MemList = 0;
	InitMemoryBlock();
	InitJobs();
	label = m_MemList;
}

CMemAllocate::~CMemAllocate()
{
	for (MemoryBlock * p = m_MemList; p!=0; p = m_MemList)
	{
		m_MemList = p->nextPointer;
		delete p;
	}
}
void CMemAllocate::DrawMemory(CDC*pDC)//ͨ�����ڴ�������ɫ��ʾ�ڴ�ķ������
{
	CPoint ptMemLeftTop = CPoint(100,50);
	CRectangle myRect(RGB(255,111,0),4,0,RGB(100,100,100),0,1,ptMemLeftTop,ptMemLeftTop);
	for (MemoryBlock * p = m_MemList; p!=0; p = p->nextPointer)
	{
		CPoint tmpLeftTop;
		tmpLeftTop.x = ptMemLeftTop.x ;
		tmpLeftTop.y = ptMemLeftTop.y + p ->StartAddr;
		CPoint tmpRightBottom;
		tmpRightBottom.x = ptMemLeftTop.x + 200;
		tmpRightBottom.y = tmpLeftTop.y + p->BlockLength;
		myRect.SetRectPeak(tmpLeftTop,tmpRightBottom);
		if (p->JobIndex  == 0 ) //����ڴ��գ� ���ǳɫ
			myRect.SetColorFill(RGB(200,200,200));
		else
			myRect.SetColorFill(RGB(100,100,100));
		
		myRect.Draw(pDC);
	}
}

void CMemAllocate::InitMemoryBlock()//��ʼ���ڴ�飬��СΪ1KB
{
	for (MemoryBlock * p = m_MemList; p!=0; p = m_MemList)
	{
		m_MemList = p->nextPointer;
		delete p;
	}	
	MemoryBlock * tmpMemBlock = new MemoryBlock;
	tmpMemBlock->BlockLength = 1024;/*ǧ�ֽ�KB*/
	tmpMemBlock->JobIndex = 0;
	tmpMemBlock->StartAddr = 0;
	tmpMemBlock->nextPointer = 0;
	tmpMemBlock->prePointer = 0;
	
	m_MemList = tmpMemBlock;
}
void CMemAllocate::AddAJobInEnd(char * JobName,int JobIndex,int JobLength,int* JobPageTable)//��ӽ���
{
	JCBInfo * tmpJob = new JCBInfo;
	tmpJob ->JobName = JobName;
	tmpJob ->JobIndex =JobIndex;
	tmpJob ->JobLength = JobLength;
	tmpJob ->JobPageTable = JobPageTable;

	tmpJob ->nextPointer = 0;
	tmpJob ->prePointer = m_JobsEnd;

	m_JobsEnd = tmpJob;
	if (m_JobsEnd ->prePointer != 0)
		m_JobsEnd ->prePointer ->nextPointer = tmpJob;
	else
		m_JobsHead = tmpJob;
}

void CMemAllocate::InitJobs()//��ʼ�������б�����Joblength��ָ����ҵ������ڴ��С����180��ʾ180�ֽ�
{
	m_JobsHead = m_JobsEnd = 0;
	AddAJobInEnd("Job1",1,180,NULL);
	AddAJobInEnd("Job2",2,360,NULL);
	AddAJobInEnd("Job3",3,180,NULL);
	AddAJobInEnd("Job4",4,60,NULL);
	AddAJobInEnd("Job5",5,020,NULL);
}


/*
ɾ��һ������
*/
void CMemAllocate::DelAJob(int JobIndex) {
	MemoryBlock* tem = m_MemList;
	//�ҵ�����
	while (true) {
		if (tem->JobIndex == JobIndex)break;
		tem = tem->nextPointer;
	}
	//ά���ڴ棬�ϲ�������
	if (tem->prePointer != 0 && tem->nextPointer != 0 && tem->prePointer->JobIndex == 0 && tem->nextPointer->JobIndex == 0) {
		tem->prePointer->BlockLength += tem->BlockLength + tem->nextPointer->BlockLength;
		tem->prePointer->nextPointer = tem->nextPointer->nextPointer;
		if (tem->nextPointer->nextPointer != 0) {
			tem->nextPointer->nextPointer->prePointer = tem->prePointer;
		}
	}
	else if (tem->prePointer != 0 && tem->prePointer->JobIndex == 0) {
		tem->prePointer->BlockLength+=tem->BlockLength;
		tem->prePointer->nextPointer=tem->nextPointer;
		if (tem->nextPointer != 0) {
			tem->nextPointer->prePointer = tem->prePointer;
		}
	}
	else if (tem->nextPointer != 0 && tem->nextPointer->JobIndex == 0) {
		tem->nextPointer->StartAddr = tem->StartAddr;
		tem->nextPointer->BlockLength += tem->BlockLength;
		tem->nextPointer->prePointer = tem->prePointer;
		if (tem->prePointer != 0) {
			tem->prePointer->nextPointer=tem->nextPointer;
		}
	}
	else tem->JobIndex = 0;
}

/*
�жϵ�ʱ�����=�������ڴ���������һ��break����ֹ���������
*/
void CMemAllocate::AllocateMem_FirstAdapt(CMemAllocate::JCBInfo& job)//ʵ�����״������㷨
{
	for (MemoryBlock * p = m_MemList; p != 0; p=p->nextPointer)
	{
		if (job.JobLength <= p->BlockLength && p->JobIndex == 0)			//�����ҵ��Ҫ�ռ�С���ڴ��ռ䣬����
		{
			MemoryBlock * tmpMemBlock = new MemoryBlock;
			tmpMemBlock ->BlockLength = job.JobLength;
			tmpMemBlock ->JobIndex = job.JobIndex;
			tmpMemBlock ->StartAddr = p->StartAddr;
			tmpMemBlock ->nextPointer = p;
			tmpMemBlock ->prePointer = p ->prePointer;
			
			p->StartAddr = p->StartAddr + job.JobLength;
			p->BlockLength = p->BlockLength -job.JobLength;
			if (p->prePointer == 0 )
			{
				m_MemList = tmpMemBlock;
			}
			else
			{
				p->prePointer->nextPointer = tmpMemBlock;
			}
			p ->prePointer = tmpMemBlock;
			break;
		}
		else
		{
			continue;
		}
	}
}

/*
�������
*/
void CMemAllocate::AllocateMem_BestAdapt(CMemAllocate::JCBInfo& job) {
	int min = 1024;
	MemoryBlock* tem;
	//�������������ڴ��
	for (MemoryBlock* p = m_MemList; p != 0; p = p->nextPointer)
	{
		//�����ҵ��Ҫ�ռ�С���ڴ��ռ䣬����
		if (job.JobLength < p->BlockLength && p->JobIndex == 0)
		{
			if (p->BlockLength <= min) {
				min = p->BlockLength;
				tem = p;
			}
		}
	}
	//�ҵ�֮������ڴ����
	if (tem != 0) {
		MemoryBlock* tmpMemBlock = new MemoryBlock;
		tmpMemBlock->BlockLength = job.JobLength;
		tmpMemBlock->JobIndex = job.JobIndex;
		tmpMemBlock->StartAddr = tem->StartAddr;
		tmpMemBlock->nextPointer = tem;
		tmpMemBlock->prePointer = tem->prePointer;

		tem->StartAddr = tem->StartAddr + job.JobLength;
		tem->BlockLength = tem->BlockLength - job.JobLength;
		if (tem->prePointer == 0)
		{
			m_MemList = tmpMemBlock;
		}
		else
		{
			tem->prePointer->nextPointer = tmpMemBlock;
		}
		tem->prePointer = tmpMemBlock;
	}
}

/*
�ٽ�����
*/
void CMemAllocate::AllocateMem_NextAdapt(CMemAllocate::JCBInfo& job) {
	//labelΪ��һ�η����ڴ��ı�־
	for (MemoryBlock* p = label; p != 0; p = p->nextPointer) {
		//������һ�η���֮����ڴ棬�������ҵ��Ŀ����
		if (job.JobLength <= p->BlockLength && p->JobIndex == 0) {
			MemoryBlock* tmpMemBlock = new MemoryBlock;
			tmpMemBlock->BlockLength = job.JobLength;
			tmpMemBlock->JobIndex = job.JobIndex;
			tmpMemBlock->StartAddr = p->StartAddr;
			tmpMemBlock->nextPointer = p;
			tmpMemBlock->prePointer = p->prePointer;

			p->StartAddr = p->StartAddr + job.JobLength;
			p->BlockLength = p->BlockLength - job.JobLength;
			if (p->prePointer == 0)
			{
				m_MemList = tmpMemBlock;
			}
			else
			{
				p->prePointer->nextPointer = tmpMemBlock;
			}
			p->prePointer = tmpMemBlock;
			label = tmpMemBlock;
			break;
		}
		else {
			continue;
		}
	}
}


void CMemAllocate::test()
{
	for (JCBInfo * p = m_JobsHead; p!= 0 ; p = p->nextPointer)
	{
		//AllocateMem_FirstAdapt(*p);
		//AllocateMem_BestAdapt(*p);
		AllocateMem_NextAdapt(*p);
	}
	//ɾ������3
	DelAJob(3);
	//�½�����6
	AddAJobInEnd("Job6", 6, 100, NULL);
	for (JCBInfo* p = m_JobsHead; p != 0; p = p->nextPointer)
	{
		//AllocateMem_FirstAdapt(*p);
		//AllocateMem_BestAdapt(*p);
		if (p->JobIndex == 6) {
			//AllocateMem_FirstAdapt(*p);
			//AllocateMem_BestAdapt(*p);
			AllocateMem_NextAdapt(*p);
		}
	}
}