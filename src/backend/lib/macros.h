/***************************************************************************
    File                 : macros.h
    Project              : SciDAVis
    Description          : Various preprocessor macros
    --------------------------------------------------------------------
    Copyright            : (C) 2008 Tilman Benkert (thzs*gmx.net)
    Copyright            : (C) 2008 Knut Franke (knut.franke*gmx.de)
                           (replace * with @ in the email addresses) 
                           
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#ifndef MACROS_H
#define MACROS_H

#define BASIC_ACCESSOR(type, var, method, Method) \
	type method() const { return var; }; \
	void set ## Method(const type value) { var = value; }
#define CLASS_ACCESSOR(type, var, method, Method) \
	type method() const { return var; }; \
	void set ## Method(const type & value) { var = value; }

#define BASIC_D_ACCESSOR_DECL(type, method, Method) \
	type method() const; \
	void set ## Method(const type value);

#define BASIC_D_ACCESSOR_IMPL(classname, type, method, Method, var) \
	void classname::set ## Method(const type value) \
	{ \
		d->var = value; \
	} \
	type classname::method() const \
	{ \
		return d->var; \
	}

#define BASIC_D_READER_IMPL(classname, type, method, var) \
	type classname::method() const \
	{ \
		return d->var; \
	}

#define BASIC_SHARED_D_READER_IMPL(classname, type, method, var) \
	type classname::method() const \
	{ \
		Q_D(const classname); \
		return d->var; \
	}

#define CLASS_D_ACCESSOR_DECL(type, method, Method) \
	type method() const; \
	void set ## Method(const type & value);

#define CLASS_D_ACCESSOR_IMPL(classname, type, method, Method, var) \
	void classname::set ## Method(const type & value) \
	{ \
		d->var = value; \
	} \
	type classname::method() const \
	{ \
		return d->var; \
	}

#define CLASS_D_READER_IMPL(classname, type, method, var) \
	type classname::method() const \
	{ \
		return d->var; \
	}

#define CLASS_SHARED_D_READER_IMPL(classname, type, method, var) \
	type classname::method() const \
	{ \
		Q_D(const classname); \
		return d->var; \
	}

#define POINTER_D_ACCESSOR_DECL(type, method, Method) \
	type *method() const; \
	void set ## Method(type *ptr);

#define FLAG_D_ACCESSOR_DECL(Method) \
	bool is ## Method() const; \
	bool has ## Method() const; \
	void set ## Method(const bool value=true); \
	void enable ## Method(const bool value=true);

#define FLAG_D_ACCESSOR_IMPL(classname, Method, var) \
	void classname::set ## Method(const bool value) \
	{ \
		d->var = value; \
	} \
	void classname::enable ## Method(const bool value) \
	{ \
		d->var = value; \
	} \
	bool classname::is ## Method() const \
	{ \
		return d->var; \
	} \
	bool classname::has ## Method() const \
	{ \
		return d->var; \
	}

#define WAIT_CURSOR QApplication::setOverrideCursor(QCursor(Qt::WaitCursor))
#define RESET_CURSOR QApplication::restoreOverrideCursor()

#define STD_SETTER_CMD_IMPL(class_name, cmd_name, value_type, field_name) \
class class_name ## cmd_name ## Cmd: public StandardSetterCmd<class_name::Private, value_type> { \
	public: \
		class_name ## cmd_name ## Cmd(class_name::Private *target, Loki::TypeTraits<value_type>::ParameterType newValue, const QString &description) \
			: StandardSetterCmd<class_name::Private, value_type>(target, &class_name::Private::field_name, newValue, description) {} \
};

#define STD_SETTER_CMD_IMPL_F(class_name, cmd_name, value_type, field_name, finalize_method) \
class class_name ## cmd_name ## Cmd: public StandardSetterCmd<class_name::Private, value_type> { \
	public: \
		class_name ## cmd_name ## Cmd(class_name::Private *target, Loki::TypeTraits<value_type>::ParameterType newValue, const QString &description) \
			: StandardSetterCmd<class_name::Private, value_type>(target, &class_name::Private::field_name, newValue, description) {} \
		virtual void finalize() { m_target->finalize_method(); } \
};

#define STD_SETTER_CMD_IMPL_I(class_name, cmd_name, value_type, field_name, init_method) \
class class_name ## cmd_name ## Cmd: public StandardSetterCmd<class_name::Private, value_type> { \
	public: \
		class_name ## cmd_name ## Cmd(class_name::Private *target, Loki::TypeTraits<value_type>::ParameterType newValue, const QString &description) \
			: StandardSetterCmd<class_name::Private, value_type>(target, &class_name::Private::field_name, newValue, description) {} \
		virtual void initialize() { m_target->init_method(); } \
};

#define STD_SETTER_CMD_IMPL_IF(class_name, cmd_name, value_type, field_name, init_method, finalize_method) \
class class_name ## cmd_name ## Cmd: public StandardSetterCmd<class_name::Private, value_type> { \
	public: \
		class_name ## cmd_name ## Cmd(class_name::Private *target, Loki::TypeTraits<value_type>::ParameterType newValue, const QString &description) \
			: StandardSetterCmd<class_name::Private, value_type>(target, &class_name::Private::field_name, newValue, description) {} \
		virtual void initialize() { m_target->init_method(); } \
		virtual void finalize() { m_target->finalize_method(); } \
};

#define STD_SWAP_METHOD_SETTER_CMD_IMPL(class_name, cmd_name, value_type, method_name) \
class class_name ## cmd_name ## Cmd: public StandardSwapMethodSetterCmd<class_name::Private, value_type> { \
	public: \
		class_name ## cmd_name ## Cmd(class_name::Private *target, Loki::TypeTraits<value_type>::ParameterType newValue, const QString &description) \
			: StandardSwapMethodSetterCmd<class_name::Private, value_type>(target, &class_name::Private::method_name, newValue, description) {} \
};

#define STD_SWAP_METHOD_SETTER_CMD_IMPL_F(class_name, cmd_name, value_type, method_name, finalize_method) \
class class_name ## cmd_name ## Cmd: public StandardSwapMethodSetterCmd<class_name::Private, value_type> { \
	public: \
		class_name ## cmd_name ## Cmd(class_name::Private *target, Loki::TypeTraits<value_type>::ParameterType newValue, const QString &description) \
			: StandardSwapMethodSetterCmd<class_name::Private, value_type>(target, &class_name::Private::method_name, newValue, description) {} \
		virtual void finalize() { m_target->finalize_method(); } \
};

#define STD_SWAP_METHOD_SETTER_CMD_IMPL_I(class_name, cmd_name, value_type, method_name, init_method) \
class class_name ## cmd_name ## Cmd: public StandardSwapMethodSetterCmd<class_name::Private, value_type> { \
	public: \
		class_name ## cmd_name ## Cmd(class_name::Private *target, Loki::TypeTraits<value_type>::ParameterType newValue, const QString &description) \
			: StandardSwapMethodSetterCmd<class_name::Private, value_type>(target, &class_name::Private::method_name, newValue, description) {} \
		virtual void initialize() { m_target->init_method(); } \
};

#define STD_SWAP_METHOD_SETTER_CMD_IMPL_IF(class_name, cmd_name, value_type, method_name, init_method, finalize_method) \
class class_name ## cmd_name ## Cmd: public StandardSwapMethodSetterCmd<class_name::Private, value_type> { \
	public: \
		class_name ## cmd_name ## Cmd(class_name::Private *target, Loki::TypeTraits<value_type>::ParameterType newValue, const QString &description) \
			: StandardSwapMethodSetterCmd<class_name::Private, value_type>(target, &class_name::Private::method_name, newValue, description) {} \
		virtual void initialize() { m_target->init_method(); } \
		virtual void finalize() { m_target->finalize_method(); } \
};

#endif // MACROS_H
