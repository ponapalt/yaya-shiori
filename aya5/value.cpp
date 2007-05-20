// 
// AYA version 5
//
// �l�������N���X�@CValue
// written by umeici. 2004
// 

#if defined(WIN32) || defined(_WIN32_WCE)
# include "stdafx.h"
#endif

#include <math.h>
#include <vector>

#include "misc.h"
#include "globaldef.h"
#include "value.h"
#include "wsex.h"

#define	__GETMAX(a, b)	( ((a) > (b)) ? (a) : (b) )
#define	__GETMIN(a, b)	( ((a) < (b)) ? (a) : (b) )

/* -----------------------------------------------------------------------
 *  �֐���  �F  CValue::GetValueInt
 *  �@�\�T�v�F  �l��int�ŕԂ��܂�
 *
 *  �^���z��ł������ꍇ��0��Ԃ��܂�
 * -----------------------------------------------------------------------
 */
int	CValue::GetValueInt(void) const
{
	switch(type) {
	case F_TAG_INT:
		return i_value;
	case F_TAG_DOUBLE:
		return (int)floor(d_value);
	case F_TAG_STRING:
		return ws_atoi(s_value, 10);
	case F_TAG_ARRAY:
		return 0;
	default:
		return 0;
	};
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CValue::GetValueDouble
 *  �@�\�T�v�F  �l��double�ŕԂ��܂�
 *
 *  �^���z��ł������ꍇ��0.0��Ԃ��܂�
 * -----------------------------------------------------------------------
 */
double	CValue::GetValueDouble(void) const
{
	switch(type) {
	case F_TAG_INT:
		return (double)i_value;
	case F_TAG_DOUBLE:
		return d_value;
	case F_TAG_STRING:
		return ws_atof(s_value);
	case F_TAG_ARRAY:
		return 0.0;
	default:
		return 0.0;
	};
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CValue::GetValueString
 *  �@�\�T�v�F  �l��yaya::string_t�ŕԂ��܂�
 *
 *  �^���z��ł������ꍇ�̓f���~�^�Ō�������������ɕϊ����ĕԂ��܂�
 * -----------------------------------------------------------------------
 */
yaya::string_t	CValue::GetValueString(void) const
{
	switch(type) {
	case F_TAG_INT: {
			yaya::string_t	result;
			ws_itoa(result, i_value, 10);
			return result;
		}
	case F_TAG_DOUBLE: {
			yaya::string_t	result;
			ws_ftoa(result, d_value);
			return result;
		}
	case F_TAG_STRING:
		return s_value;
	case F_TAG_ARRAY: {
			yaya::string_t	result;
			for(std::vector<CValueSub>::const_iterator it = array().begin();
				it != array().end(); it++) {
				if (it != array().begin())
					result += VAR_DELIMITER;
				yaya::string_t	tmpstr = it->GetValueString();
				result += tmpstr;
			}
			return result;
		}
	default:
		return yaya::string_t(L"");
	};
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CValue::GetValueStringForLogging
 *  �@�\�T�v�F  �l��yaya::string_t�ŕԂ��܂��i���K�[�p�j
 *
 *  GetValueString�Ƃ̈Ⴂ�́A��������_�u���N�H�[�g���邩�ۂ��ł��B
 * -----------------------------------------------------------------------
 */
yaya::string_t	CValue::GetValueStringForLogging(void) const
{
	switch(type) {
	case F_TAG_INT: {
			yaya::string_t	result;
			ws_itoa(result, i_value, 10);
			return result;
		}
	case F_TAG_DOUBLE: {
			yaya::string_t	result;
			ws_ftoa(result, d_value);
			return result;
		}
	case F_TAG_STRING: {
			yaya::string_t	result = s_value;
			AddDoubleQuote(result);
			return result;
		}
	case F_TAG_ARRAY: {
			yaya::string_t	result;
			for(std::vector<CValueSub>::const_iterator it = array().begin();
				it != array().end(); it++) {
				if (it != array().begin())
					result += VAR_DELIMITER;
				yaya::string_t	tmpstr = it->GetValueString();
				if (it->GetType() == F_TAG_STRING)
					AddDoubleQuote(tmpstr);
				result += tmpstr;
			}
			return result;
		}
	default:
		return yaya::string_t(L"");
	};
}
/* -----------------------------------------------------------------------
 *  �֐���  �F  CValue::SetArrayValue
 *  �@�\�T�v�F  �z��̎w�肵���ʒu�֒l��ݒ肵�܂��B�K�v�ɉ����Č^�ϊ����s���܂�
 *
 *  ���̌^���ȈՔz��Ɣėp�z��̏ꍇ�͂��̂܂܏������܂����A����/�����������ꍇ��
 *  �ėp�z��Ɍ^�ϊ�����A���̒l��[0]�Ɋi�[����܂��B
 * -----------------------------------------------------------------------
 */
void	CValue::SetArrayValue(const CValue &oval, const CValue &value)
{
	// �^�ϊ����Č��̒l��[0]�֑��
	if (type == F_TAG_INT) {
		CValueSub	addvs(i_value);
		std::vector<CValueSub>	addvsarray;
		addvsarray.push_back(addvs);
		*this = addvsarray;
	}
	else if (type == F_TAG_DOUBLE) {
		CValueSub	addvs(d_value);
		std::vector<CValueSub>	addvsarray;
		addvsarray.push_back(addvs);
		*this = addvsarray;
	}

	// �����ƃf���~�^�̎擾
	int	order, order1;
	yaya::string_t	delimiter;
	int	aoflg = oval.DecodeArrayOrder(order, order1, delimiter);

	// �l���X�V����
	if (type == F_TAG_STRING) {
		// �ȈՔz��

		// ����l��������łȂ��ꍇ�͉������Ȃ�
		if (value.GetType() != F_TAG_STRING)
			return;
		// ���̕�������f���~�^�ŕ���
		std::vector<yaya::string_t>	s_array;
		int	sz = SplitToMultiString(s_value, s_array, delimiter);
		// �X�V
		if (aoflg) {
			// �͈͂�
			if (order1 < 0)
				return;
			else if (order < sz) {
				int	s_index   = __GETMAX(order, 0);
				int	e_index   = __GETMIN(order1 + 1, sz);
				int	s_indexp1 = s_index + 1;
				int	i         = 0;
				s_array[s_index] = value.s_value;
				for(std::vector<yaya::string_t>::iterator it = s_array.begin();
					it != s_array.end(); i++) {
					if (s_indexp1 <= i && i < e_index)
						it = s_array.erase(it);
					else if (i >= e_index)
						break;
					else
						it++;
				}
			}
			else {
				int	addsize = order - s_array.size();
				for(int i = 0; i < addsize; i++) {
					yaya::string_t	addstr;
					s_array.push_back(addstr);
				}
				s_array.push_back(value.s_value);
			}
		}
		else {
			// �͈͂Ȃ�
			if (order < 0)
				return;
			else if (order < sz)
				s_array[order] = value.s_value;
			else {
				int	addsize = order - sz;
				for(int i = 0; i < addsize; i++) {
					yaya::string_t	addstr;
					s_array.push_back(addstr);
				}
				s_array.push_back(value.s_value);
			}
		}
		// ������̕���
		sz = s_array.size();
		if (!sz)
			s_value = L"";
		else {
			s_value = s_array[0];
			for(int i = 1; i < sz; i++)
				s_value += delimiter + s_array[i];
		}
	}
	else if (type == F_TAG_ARRAY || type == F_TAG_VOID) {
		// �ėp�z��i�������͖��������j
		type = F_TAG_ARRAY;

		if (aoflg) {
			int	sz = array_size();
			// �͈͂�
			if (order1 < 0)
				return;
			if (order < sz) {
				// �z�񒆓r�̏�������
				int	s_index   = __GETMAX(order, 0);
				int	e_index   = __GETMIN(order1 + 1, sz);
				int	i         = 0;
				if (value.GetType() == F_TAG_INT)
					array()[s_index] = value.i_value;
				else if (value.GetType() == F_TAG_DOUBLE)
					array()[s_index] = value.d_value;
				else if (value.GetType() == F_TAG_STRING)
					array()[s_index] = value.s_value;
				else if (value.GetType() == F_TAG_ARRAY) {
					i = 0;
					for(std::vector<CValueSub>::iterator it = array().begin(); it != array().end(); it++, i++)
						if (i == s_index) {
							it = array().erase(it);
							array().insert(it, value.array().begin(), value.array().end());
							s_index += (value.array_size() - 1);
							e_index += (value.array_size() - 1);
							break;
						}
				}
				i = 0;
				for(std::vector<CValueSub>::iterator it = array().begin(); it != array().end(); i++) {
					if (s_index < i && i < e_index)
						it = array().erase(it);
					else if (i >= e_index)
						break;
					else
						it++;
				}
			}
			else {
				// ��[�ւ̒ǉ�
				std::vector<CValueSub>	*t_array = &array();
				int	addsize = order - t_array->size();
				for(int i = 1; i <= addsize; i++) {
					CValueSub	addvs;
					t_array->push_back(addvs);
				}
				if (value.GetType() == F_TAG_INT)
					t_array->push_back(value.i_value);
				else if (value.GetType() == F_TAG_DOUBLE)
					t_array->push_back(value.d_value);
				else if (value.GetType() == F_TAG_STRING)
					t_array->push_back(value.s_value);
				else if (value.GetType() == F_TAG_ARRAY)
					t_array->insert(t_array->end(),
						value.array().begin(), value.array().end());
			}
		}
		else {
			// �͈͂Ȃ�
			if (order < 0)
				return;
			if (order < static_cast<int>(array_size()) ) {
				// �z�񒆓r�̏�������
				if (value.GetType() == F_TAG_INT)
					array()[order] = value.i_value;
				else if (value.GetType() == F_TAG_DOUBLE)
					array()[order] = value.d_value;
				else if (value.GetType() == F_TAG_STRING)
					array()[order] = value.s_value;
				else if (value.GetType() == F_TAG_ARRAY || value.GetType() == F_TAG_VOID) {
					for(std::vector<CValueSub>::iterator it = array().begin(); it != array().end(); it++, order--)
						if (!order) {
							it = array().erase(it);
							if ( value.GetType() == F_TAG_ARRAY ) {
								array().insert(it, value.array().begin(), value.array().end());
							}
							break;
						}
				}
			}
			else {
				// ��[�ւ̒ǉ�
				std::vector<CValueSub>	*t_array = &array();
				int	addsize = order - t_array->size();
				for(int i = 1; i <= addsize; i++) {
					CValueSub	addvs;
					t_array->push_back(addvs);
				}
				if (value.GetType() == F_TAG_INT)
					t_array->push_back(value.i_value);
				else if (value.GetType() == F_TAG_DOUBLE)
					t_array->push_back(value.d_value);
				else if (value.GetType() == F_TAG_STRING)
					t_array->push_back(value.s_value);
				else if (value.GetType() == F_TAG_ARRAY)
					t_array->insert(t_array->end(),
						value.array().begin(), value.array().end());
			}
		}
	}
}

/* -----------------------------------------------------------------------
 *  �֐���  �F  CValue::DecodeArrayOrder
 *  �@�\�T�v�F  ����̏����Ŕz�񏘐����i�[���Ă���this���珘���ƃf���~�^���擾���܂�
 *
 *  �Ԓl�@�@�F  0/1=order1(�͈͎w��)����/�L��
 * -----------------------------------------------------------------------
 */
int	CValue::DecodeArrayOrder(int &order, int &order1, yaya::string_t &delimiter) const
{
	order  = 0;
	order1 = 0;
	delimiter = VAR_DELIMITER;

	if (type == F_TAG_ARRAY) {
		int	sz = array_size();
		if (sz) {
			// �v�f0:����
			if (array()[0].GetType() == F_TAG_INT)
				order = array()[0].i_value;
			else if (array()[0].GetType() == F_TAG_DOUBLE)
				order = (int)floor(array()[0].d_value);
			else
				return 0;
			if (sz == 1)
				return 0;
			// �v�f1:���l�Ȃ�͈͎w��A������Ȃ�f���~�^
			switch(array()[1].GetType()) {
			case F_TAG_INT:
				order1  = array()[1].i_value;
				if (order > order1)
					exchange(order, order1);
				break;
			case F_TAG_DOUBLE:
				order1  = (int)floor(array()[1].d_value);
				if (order > order1)
					exchange(order, order1);
				break;
			case F_TAG_STRING:
				if (array()[1].s_value.size())
					delimiter = array()[1].s_value;
				return 0;
			default:
				return 0;
			};
			if (sz == 2)
				return 1;
			// �v�f2:�v�f1���͈͎w�肾�����ꍇ�̓f���~�^
			if (array()[2].GetType() == F_TAG_STRING &&
				array()[2].s_value.size())
				delimiter = array()[2].s_value;
			return 1;
		}
	}

	return 0;
}

/* -----------------------------------------------------------------------
 *  operator = (int)
 * -----------------------------------------------------------------------
 */
CValue &CValue::operator =(int value)
{
	type    = F_TAG_INT;
	i_value = value;

	return *this;
}

/* -----------------------------------------------------------------------
 *  operator = (double)
 * -----------------------------------------------------------------------
 */
CValue &CValue::operator =(double value)
{
	type    = F_TAG_DOUBLE;
	d_value = value;

	return *this;
}

/* -----------------------------------------------------------------------
 *  operator = (yaya::string_t)
 * -----------------------------------------------------------------------
 */
CValue &CValue::operator =(const yaya::string_t &value)
{
	type    = F_TAG_STRING;
	s_value = value;

	return *this;
}

/* -----------------------------------------------------------------------
 *  operator = (yaya::char_t*)
 * -----------------------------------------------------------------------
 */
CValue &CValue::operator =(const yaya::char_t *value)
{
	type    = F_TAG_STRING;
	s_value = value;

	return *this;
}

/* -----------------------------------------------------------------------
 *  operator = (std::vector<CValueSub>)
 * -----------------------------------------------------------------------
 */
CValue &CValue::operator =(const std::vector<CValueSub> &value)
{
	type    = F_TAG_ARRAY;
	array().assign(value.begin(), value.end());

	return *this;
}

/* -----------------------------------------------------------------------
 *  operator = (CValueSub)
 * -----------------------------------------------------------------------
 */
CValue &CValue::operator =(const CValueSub &value)
{
	switch(value.GetType()) {
	case F_TAG_INT:
		*this = value.i_value;
		break;
	case F_TAG_DOUBLE:
		*this = value.d_value;
		break;
	case F_TAG_STRING:
		*this = value.s_value;
		break;
	case F_TAG_VOID:
		type = F_TAG_VOID;
		i_value = 0;
		d_value = 0;
		s_value = L"";
		break;
	default:
		*this = L"";
		break;
	};

	return *this;
}

/* -----------------------------------------------------------------------
 *  GetValueSub
 *
 *  CValueSub������܂��B�z�񉉎Z�p�B
 * -----------------------------------------------------------------------
 */
CValueSub CValue::GetValueSub(void) const
{
	switch(type) {
	case F_TAG_INT:
		return CValueSub(i_value);
	case F_TAG_DOUBLE:
		return CValueSub(d_value);
	case F_TAG_STRING:
		return CValueSub(s_value);
	case F_TAG_ARRAY:
		return CValueSub(GetValueString());
	}
	return CValueSub();
}

/* -----------------------------------------------------------------------
 *  CalcEscalationTypeNum
 *
 *  �^�̏��i���[���������܂��i���l�D��j
 *  ��{�I��ARRAY>DOUBLE>INT�ł��B
 * -----------------------------------------------------------------------
 */
int CValue::CalcEscalationTypeNum(const int rhs) const
{
	int result = type > rhs ? type : rhs;
	if ( result != F_TAG_STRING ) { return result; }

	switch ( type <= rhs ? type : rhs ) {
	case F_TAG_VOID:
	case F_TAG_INT:
		return F_TAG_INT;
	case F_TAG_DOUBLE:
	case F_TAG_STRING:
		return F_TAG_DOUBLE;
	case F_TAG_ARRAY:
		return F_TAG_ARRAY;
	}
	return F_TAG_VOID;
}

/* -----------------------------------------------------------------------
 *  CalcEscalationTypeStr
 *
 *  �^�̏��i���[���������܂��i������D��j
 *  ��{�I��ARRAY>STRING>DOUBLE>INT>VOID�ł��B
 *  �^��#define���ɂ����Ȃ�悤�ɒ�`����Ă��邽�߁A�l��r�ő傫���ق���Ԃ��܂��B
 * -----------------------------------------------------------------------
 */
int CValue::CalcEscalationTypeStr(const int rhs) const
{
	return type > rhs ? type : rhs;
}

/* -----------------------------------------------------------------------
 *  operator + (CValue)
 * -----------------------------------------------------------------------
 */
CValue CValue::operator +(const CValue &value) const
{
	int t = CalcEscalationTypeStr(value.type);
	
	switch(t) {
	case F_TAG_INT:
		return CValue(GetValueInt() + value.GetValueInt());
	case F_TAG_DOUBLE:
		return CValue(GetValueDouble() + value.GetValueDouble());
	case F_TAG_STRING:
		return CValue(GetValueString() + value.GetValueString());
	case F_TAG_ARRAY: {
			CValue result;
			if (type == F_TAG_ARRAY && value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				std::vector<CValueSub>::const_iterator it, it2;
				for(it = array().begin(), it2 = value.array().begin();
					it != array().end() && it2 != value.array().end(); it++, it2++)
					result.array().push_back((*it) + (*it2));
			}
			else if (type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(value.GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = array().begin(); it != array().end(); it++)
					result.array().push_back(*it + t_vs);
				result.type = F_TAG_ARRAY;
			}
			else if (value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = value.array().begin(); it != value.array().end(); it++)
					result.array().push_back(t_vs + *it);
			}
			return result;
		}
	};
	
	return CValue(value);
}

/* -----------------------------------------------------------------------
 *  operator - (CValue)
 * -----------------------------------------------------------------------
 */
CValue CValue::operator -(const CValue &value) const
{
	int t = CalcEscalationTypeNum(value.type);
	
	switch(t) {
	case F_TAG_INT:
		return CValue(GetValueInt() - value.GetValueInt());
	case F_TAG_DOUBLE:
		return CValue(GetValueDouble() - value.GetValueDouble());
	case F_TAG_ARRAY: {
			CValue result;
			if (type == F_TAG_ARRAY && value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				std::vector<CValueSub>::const_iterator it, it2;
				for(it = array().begin(), it2 = value.array().begin();
					it != array().end() && it2 != value.array().end(); it++, it2++)
					result.array().push_back((*it) - (*it2));
			}
			else if (type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(value.GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = array().begin(); it != array().end(); it++)
					result.array().push_back(*it - t_vs);
				result.type = F_TAG_ARRAY;
			}
			else if (value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = value.array().begin(); it != value.array().end(); it++)
					result.array().push_back(t_vs - *it);
			}
			return result;
		}
	};
	
	return CValue(value);
}

/* -----------------------------------------------------------------------
 *  operator * (CValue)
 * -----------------------------------------------------------------------
 */
CValue CValue::operator *(const CValue &value) const
{
	int t = CalcEscalationTypeNum(value.type);
	
	switch(t) {
	case F_TAG_INT:
		return CValue(GetValueInt() * value.GetValueInt());
	case F_TAG_DOUBLE:
		return CValue(GetValueDouble() * value.GetValueDouble());
	case F_TAG_ARRAY: {
			CValue result;
			if (type == F_TAG_ARRAY && value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				std::vector<CValueSub>::const_iterator it, it2;
				for(it = array().begin(), it2 = value.array().begin();
					it != array().end() && it2 != value.array().end(); it++, it2++)
					result.array().push_back((*it) * (*it2));
			}
			else if (type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(value.GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = array().begin(); it != array().end(); it++)
					result.array().push_back(*it * t_vs);
				result.type = F_TAG_ARRAY;
			}
			else if (value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = value.array().begin(); it != value.array().end(); it++)
					result.array().push_back(t_vs * *it);
			}
			return result;
		}
	};
	
	return CValue(value);
}

/* -----------------------------------------------------------------------
 *  operator / (CValue)
 * -----------------------------------------------------------------------
 */
CValue CValue::operator /(const CValue &value) const
{
	int t = CalcEscalationTypeNum(value.type);
	
	switch(t) {
	case F_TAG_INT:
		{
			int denom = value.GetValueInt();
			if ( denom ) {
				return CValue(GetValueInt() / denom);
			}
			else {
				return CValue(GetValueInt());
			}
		}
	case F_TAG_DOUBLE:
		{
			double denom = value.GetValueDouble();
			if ( denom ) {
				return CValue(GetValueDouble() / denom);
			}
			else {
				return CValue(GetValueDouble());
			}
		}
	case F_TAG_ARRAY: {
			CValue result;
			if (type == F_TAG_ARRAY && value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				std::vector<CValueSub>::const_iterator it, it2;
				for(it = array().begin(), it2 = value.array().begin();
					it != array().end() && it2 != value.array().end(); it++, it2++)
					result.array().push_back((*it) / (*it2));
			}
			else if (type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(value.GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = array().begin(); it != array().end(); it++)
					result.array().push_back(*it / t_vs);
				result.type = F_TAG_ARRAY;
			}
			else if (value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = value.array().begin(); it != value.array().end(); it++)
					result.array().push_back(t_vs / *it);
			}
			return result;
		}
	};
	
	return CValue(value);
}

/* -----------------------------------------------------------------------
 *  operator % (CValue)
 * -----------------------------------------------------------------------
 */
CValue CValue::operator %(const CValue &value) const
{
	int t = CalcEscalationTypeNum(value.type);
	
	switch(t) {
	case F_TAG_INT:
	case F_TAG_DOUBLE:
		{
			int denom = value.GetValueInt();
			if ( denom ) {
				return CValue(GetValueInt() % denom);
			}
			else {
				return CValue(GetValueInt());
			}
		}
	case F_TAG_ARRAY: {
			CValue result;
			if (type == F_TAG_ARRAY && value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				std::vector<CValueSub>::const_iterator it, it2;
				for(it = array().begin(), it2 = value.array().begin();
					it != array().end() && it2 != value.array().end(); it++, it2++)
					result.array().push_back((*it) % (*it2));
			}
			else if (type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(value.GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = array().begin(); it != array().end(); it++)
					result.array().push_back(*it % t_vs);
				result.type = F_TAG_ARRAY;
			}
			else if (value.type == F_TAG_ARRAY) {
				result.type = F_TAG_ARRAY;
				const CValueSub t_vs(GetValueSub());
				for(std::vector<CValueSub>::const_iterator it = value.array().begin(); it != value.array().end(); it++)
					result.array().push_back(t_vs % *it);
			}
			return result;
		}
	};
	
	return CValue(value);
}

/* -----------------------------------------------------------------------
 *  operator [] (CValue)
 *
 *  this�̌^��yaya::string_t�̏ꍇ�͊ȈՔz��Aarray()�̏ꍇ�͔z�񈵂��ł��B
 *  int/double�ł͏����ɂ�炸���̒l���Ԃ���܂��B
 *
 *  �������͈͊O�̏ꍇ�͋󕶎����Ԃ��܂��B
 *
 *  �����̌^�͏��array()�ł���A����̃t�H�[�}�b�g�ɏ������Ă���K�v������܂��B
 *  �i�Ăяo�����ł��̂悤�ɐ��`����K�v������܂��j
 * -----------------------------------------------------------------------
 */
CValue CValue::operator [](const CValue &value) const
{
	int	order, order1;
	yaya::string_t	delimiter;
	int	aoflg = value.DecodeArrayOrder(order, order1, delimiter);

	if (type == F_TAG_INT || type == F_TAG_DOUBLE) {
		// ���l�@������0�Ȃ�this�A1�ȊO�ł͋󕶎����Ԃ�
		if (!order)
			return *this;
		else
			return CValue();
	}
	if (type == F_TAG_STRING) {
		// �ȈՔz��

		// ��������f���~�^�ŕ���
		std::vector<yaya::string_t>	s_array;
		int	sz = SplitToMultiString(s_value, s_array, delimiter);
		// �l�̎擾
		if (aoflg) {
			// �͈͂���
			if (order1 < 0 || order >= sz)
				return CValue();
			else {
				int	s_index   = __GETMAX(order, 0);
				int	e_index   = __GETMIN(order1 + 1, sz);
				int	i         = 0;
				int	j         = 0;
				yaya::string_t	result_str;
				for(std::vector<yaya::string_t>::iterator it = s_array.begin();
					it != s_array.end(); it++, i++) {
					if (s_index <= i && i < e_index) {
						if (j)
							result_str += delimiter;
						j = 1;
						result_str += *it;
					}
					else if (i >= e_index)
						break;
				}
				return result_str;
			}
		}
		else {
			// �͈͂Ȃ�
			if (0 <= order && order < sz)
				return CValue(s_array[order]);
			else 
				return CValue();
		}
	}
	else if (type == F_TAG_ARRAY) {
		// �ėp�z��

		int	sz = array_size();
		// �l�̎擾
		if (aoflg) {
			// �͈͂���
			if (order1 < 0 || order >= sz)
				return CValue(F_TAG_ARRAY, 0/*dmy*/);
			else {
				int	s_index   = __GETMAX(order, 0);
				int	e_index   = __GETMIN(order1 + 1, sz);
				int	i         = 0;
				CValue	result_array(F_TAG_ARRAY, 0/*dmy*/);
				for(std::vector<CValueSub>::const_iterator it = array().begin();
					it != array().end(); it++, i++) {
					if (s_index <= i && i < e_index)
						result_array.array().push_back(*it);
					else if (i >= e_index)
						break;
				}
				return result_array;
			}
		}
		else {
			// �͈͂Ȃ�
			if (0 <= order && order < sz) {
				return CValue(array()[order]);
			}
			else {
				return CValue(L"");
			}
		}
	}

	return CValue(L"");
}

/* -----------------------------------------------------------------------
 *  operator == / != (CValue)
 * -----------------------------------------------------------------------
 */
int CValue::Compare(const CValue &value) const
{
	int t = CalcEscalationTypeStr(value.type);

	switch(t) {
	case F_TAG_VOID:
		return 1; //VOID�^���m�̉��Z��TRUE
	case F_TAG_INT:
		return GetValueInt() == value.GetValueInt();
	case F_TAG_DOUBLE:
		return GetValueDouble() == value.GetValueDouble();
	case F_TAG_STRING:
		return GetValueString() == value.GetValueString();
	case F_TAG_ARRAY: {
			if (value.type == F_TAG_ARRAY) {
				size_t	len = array_size();
				if (len != value.array_size())
					return 0;
				else {
					std::vector<CValueSub>::const_iterator it, it2;
					size_t	i = 0;
					for(it = array().begin(), it2 = value.array().begin();
						it != array().end() && it2 != value.array().end(); it++, it2++)		
						i += it->Compare(*it2);
					return (i == len);
				}
			}
			else {
				return 0;
			}
		}
	}

	return 0;
}

/* -----------------------------------------------------------------------
 *  operator > | <= (CValue)
 * -----------------------------------------------------------------------
 */
int CValue::Great(const CValue &value) const
{
	int t = CalcEscalationTypeStr(value.type);

	if (t == F_TAG_INT) {
		return GetValueInt() > value.GetValueInt();
	}
	else if (t == F_TAG_DOUBLE) {
		return GetValueDouble() > value.GetValueDouble();
	}
	else if (t == F_TAG_STRING) {
		return GetValueString() > value.GetValueString();
	}
	return 0;

}

/* -----------------------------------------------------------------------
 *  operator < / >= (CValue)
 * -----------------------------------------------------------------------
 */
int CValue::Less(const CValue &value) const
{
	int t = CalcEscalationTypeStr(value.type);

	if (t == F_TAG_INT) {
		return GetValueInt() < value.GetValueInt();
	}
	else if (t == F_TAG_DOUBLE) {
		return GetValueDouble() < value.GetValueDouble();
	}
	else if (t == F_TAG_STRING) {
		return GetValueString() < value.GetValueString();
	}
	return 0;
}
