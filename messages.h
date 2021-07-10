// 
// AYA version 5
//
// ���O���b�Z�[�W
// written by umeici. 2004
// 
// �p��̃��b�Z�[�W�͊�{�I�Ɉȉ��̃T�C�g���Ŏ����|�󂵂����̂ł��B
// excite�|��
// http://www.excite.co.jp/world/
//

#ifndef	MESSAGESH
#define	MESSAGESH

//----
#include <vector>
#include <string>
#include "globaldef.h"
// ���b�Z�[�W���
#define	E_I			0	/* info */
#define	E_F			1	/* fatal */
#define	E_E			2	/* error */
#define	E_W			3	/* warning */
#define	E_N			4	/* note */
#define E_J			5	/* other(j) */
#define	E_END		6	/* ���O�̏I�� */
#define	E_SJIS		16	/* �}���`�o�C�g�����R�[�h��SJIS */
#define	E_UTF8		17	/* �}���`�o�C�g�����R�[�h��UTF-8 */
#define	E_DEFAULT	32	/* �}���`�o�C�g�����R�[�h��OS�f�t�H���g�̃R�[�h */

namespace yayamsg {

bool LoadMessageFromTxt(const yaya::string_t &file,char cset);
const yaya::string_t GetTextFromTable(int mode,int id);
bool IsEmpty(void);

typedef std::vector<yaya::string_t> MessageArray;

// �t�F�[�^���G���[������
extern MessageArray msgf;

// �G���[������
extern MessageArray msge;

// ���[�j���O������
extern MessageArray msgw;

// ���L������
extern MessageArray msgn;

// ���̑��̃��O������
extern MessageArray msgj;

}

#endif
