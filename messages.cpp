// 
// AYA version 5
//
// ���O���b�Z�[�W
// written by umeici. 2004
// 

#include "messages.h"

#include "globaldef.h"
#include "wsex.h"

static void ClearMessageArrays()
{
	yayamsg::msgf.clear();
	yayamsg::msge.clear();
	yayamsg::msgw.clear();
	yayamsg::msgn.clear();
	yayamsg::msgj.clear();
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  yayamsg::IsEmpty
 *  �@�\�T�v�F  �G���[���b�Z�[�W�e�[�u�����󂩂ǂ����𔻒肵�܂�
 * -----------------------------------------------------------------------
 */
bool yayamsg::IsEmpty(void)
{
	return yayamsg::msgf.empty() && yayamsg::msge.empty() && yayamsg::msgw.empty();
}


/* -----------------------------------------------------------------------
 *  �֐���  �F  yayamsg::LoadMessageFromTxt
 *  �@�\�T�v�F  �e�L�X�g�t�@�C������G���[���b�Z�[�W�e�[�u���ɓǂݍ��݂܂�
 * -----------------------------------------------------------------------
 */
bool yayamsg::LoadMessageFromTxt(const yaya::string_t &file,char cset)
{
	FILE *fp = yaya::w_fopen(file.c_str(), L"rb");

	if (fp == NULL) {
		return false;
	}

	MessageArray *ptr = NULL;

	yaya::string_t line;

	ClearMessageArrays();

	while ( true )
	{
		if (yaya::ws_fgets(line, fp, cset, 0 /*no_enc*/, 1 /*skip_bom*/, 1 /*cut_heading_space*/) == yaya::WS_EOF) {
			break;
		}

		if ( line.substr(0,3)==L"!!!" ) {
			yaya::string_t&type = line.substr(3);

			if ( type == L"msgf" ) {
				ptr = &msgf;
			}
			else if ( type == L"msge" ) {
				ptr = &msge;
			}
			else if ( type == L"msgw" ) {
				ptr = &msgw;
			}
			else if ( type == L"msgn" ) {
				ptr = &msgn;
			}
			else if ( type == L"msgj" ) {
				ptr = &msgj;
			}
			else {
				ptr = NULL;
			}
			continue;
		}
		
		if ( line.substr(0,2)==L"//" ) {
			continue;
		}
		
		if ( line.substr(0,1)==L"*" ) {
			if ( ptr ) {
				line=line.substr(1);
				yaya::ws_replace(line,L"\\n", L"\r\n");
				ptr->push_back(line);
			}
			continue;
		}
	}

	fclose(fp);

	return true;
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  yayamsg::GetTextFromTable
 *  �@�\�T�v�F  �G���[���b�Z�[�W�e�[�u�����當����𔲂��o���܂��B
 * -----------------------------------------------------------------------
 */
const yaya::string_t yayamsg::GetTextFromTable(int mode,int id)
{
	yayamsg::MessageArray *ptr;
	yaya::char_t *emsg;

	if (mode == E_F) {
		ptr = &yayamsg::msgf;
		emsg = L"fatal F";
	}
	else if (mode == E_E) {
		ptr = &yayamsg::msge;
		emsg = L"error E";
	}
	else if (mode == E_W) {
		ptr = &yayamsg::msgw;
		emsg = L"warning W";
	}
	else if (mode == E_N) {
		ptr = &yayamsg::msgn;
		emsg = L"note N";
	}
	else {
		ptr = &yayamsg::msgj;
		emsg = L"//msg M";
	}

	if ( id < 0 || ptr->size() <= static_cast<size_t>(id) ) { //catch overflow
		yaya::char_t buf[64] = L"";
		yaya::snprintf(buf,63,L"%04d",id);

		return yaya::string_t(emsg) + buf + L" : (please specify messagetxt)\r\n";
	}
	else {
		return (*ptr)[id];
	}
}

namespace yayamsg {

	// fatal
	MessageArray msgf;

	// error
	MessageArray msge;

	// warning
	MessageArray msgw;

	// note
	MessageArray msgn;

	// other
	MessageArray msgj;

}
