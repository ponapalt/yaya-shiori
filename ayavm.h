// 
// AYA version 5
//
// AYA��1�C���X�^���X��ێ�����N���XAYAVM
// written by the Maintenance Shop/C.Ponapalt 2006
// 
// CAyaVM������������ƕ�����AYA��1�̃v���Z�X/�X���b�h/���W���[�����ő��点�邱�Ƃ��ł��܂��B
// 

#ifndef AYAVM_H
#define AYAVM_H

#include <vector>
#include <map>
#include <memory>
#include "log.h"
#include "mt19937ar.h"
#include "fix_old_compiler.h"

class CBasis;
class CFunction;
class CCallDepth;
class CSystemFunction;
class CGlobalVariable;
class CFile;
class CLib;
class CParser0;
class CParser1;
class CDefine;
class CAyaVM;

class CAyaVM
{
private:
	std_shared_ptr<CBasis>					m_basis;

	std_shared_ptr< std::vector<CFunction> >	m_function;
	std_shared_ptr< yaya::indexmap > m_functionmap;
	
	std_shared_ptr< std::vector<CDefine> >	m_gdefines;

	std_shared_ptr<CCallDepth>				m_calldepth;
	std_shared_ptr<CSystemFunction>			m_sysfunction;
	std_shared_ptr<CGlobalVariable>			m_variable;

	std_shared_ptr<CFile>					m_files;
	std_shared_ptr<CLib>						m_libs;

	std_shared_ptr<CParser0>					m_parser0;
	std_shared_ptr<CParser1>					m_parser1;

	CLog	m_logger;

	MersenneTwister rs_sysfunc;
	MersenneTwister rs_internal;

public:
	CAyaVM() {}
	CAyaVM(CAyaVM &vm);
	virtual ~CAyaVM() {}

	CAyaVM* get_a_deep_copy();

	void Load(void);
	void Unload(void);

	unsigned int genrand(void);
	int genrand_int(int n);

	unsigned int genrand_sysfunc(void);
	int genrand_sysfunc_int(int n);

	void genrand_sysfunc_srand(int n);
	void genrand_sysfunc_srand_array(const unsigned long a[],const int n);

	// �吧��
	CBasis&					basis();

	// �֐�/�V�X�e���֐�/�O���[�o���ϐ�
	std::vector<CFunction>&	function();
	yaya::indexmap& functionmap();

	std::vector<CDefine>&	gdefines();

	CCallDepth&				calldepth();
	CSystemFunction&		sysfunction();
	CGlobalVariable&		variable();

	// �t�@�C���ƊO�����C�u����
	CFile&					files();
	CLib&					libs();

	// ���K�[
	inline CLog& logger() {
		return m_logger;
	}

	// �p�[�T
	CParser0&				parser0();
	CParser1&				parser1();
};

#endif //AYAVM_H


