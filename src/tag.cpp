#include "tag.h"

using namespace Nan;

Tag::Tag(MifareTag tag) : tag(tag) {}
Tag::~Tag() {}

MifareTag Tag::constructorTag = NULL;

// TODO free tag on delete

NAN_MODULE_INIT(Tag::Init) {
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(Tag::New);
	tpl->SetClassName(Nan::New("Tag").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	Nan::SetPrototypeMethod(tpl, "getTagType", Tag::GetTagType);
	Nan::SetPrototypeMethod(tpl, "getTagFriendlyName", Tag::GetTagFriendlyName);
	Nan::SetPrototypeMethod(tpl, "getTagUID", Tag::GetTagUID);


	Nan::SetPrototypeMethod(tpl, "mifareUltralight_connect", Tag::mifareUltralight_connect);
	Nan::SetPrototypeMethod(tpl, "mifareUltralight_disconnect", Tag::mifareUltralight_disconnect);
	Nan::SetPrototypeMethod(tpl, "mifareUltralight_read", Tag::mifareUltralight_read);
	Nan::SetPrototypeMethod(tpl, "mifareUltralight_write", Tag::mifareUltralight_write);


	Nan::SetPrototypeMethod(tpl, "mifareClassic_connect", Tag::mifareClassic_connect);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_disconnect", Tag::mifareClassic_disconnect);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_authenticate", Tag::mifareClassic_authenticate);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_read", Tag::mifareClassic_read);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_initValue", Tag::mifareClassic_initValue);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_readValue", Tag::mifareClassic_readValue);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_write", Tag::mifareClassic_write);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_increment", Tag::mifareClassic_increment);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_decrement", Tag::mifareClassic_decrement);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_restore", Tag::mifareClassic_restore);
	Nan::SetPrototypeMethod(tpl, "mifareClassic_transfer", Tag::mifareClassic_transfer);

	Nan::SetPrototypeMethod(tpl, "mifareDesfire_connect", Tag::mifareDesfire_connect);
	Nan::SetPrototypeMethod(tpl, "mifareDesfire_disconnect", Tag::mifareDesfire_disconnect);
	Nan::SetPrototypeMethod(tpl, "mifareDesfire_authenticate_des", Tag::mifareDesfire_authenticate_des);
	Nan::SetPrototypeMethod(tpl, "mifareDesfire_authenticate_3des", Tag::mifareDesfire_authenticate_3des);
	Nan::SetPrototypeMethod(tpl, "mifareDesfire_getApplicationIds", Tag::mifareDesfire_getApplicationIds);
	Nan::SetPrototypeMethod(tpl, "mifareDesfire_selectApplication", Tag::mifareDesfire_selectApplication);
	Nan::SetPrototypeMethod(tpl, "mifareDesfire_getFileIds", Tag::mifareDesfire_getFileIds);
	Nan::SetPrototypeMethod(tpl, "mifareDesfire_write", Tag::mifareDesfire_write);
	Nan::SetPrototypeMethod(tpl, "mifareDesfire_read", Tag::mifareDesfire_read);


	constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
	Nan::Set(target, Nan::New("Tag").ToLocalChecked(),
	Nan::GetFunction(tpl).ToLocalChecked());
}

Nan::Persistent<v8::Function> & Tag::constructor() {
	static Nan::Persistent<v8::Function> my_constructor;
	return my_constructor;
}

NAN_METHOD(Tag::New) {
	std::cout << "New hello 1" << std::endl;
	if (info.IsConstructCall()) {
		std::cout << "New hello ConstructCall 1" << std::endl;
		std::cout << "New hello ConstructCall tagname:" << freefare_get_tag_friendly_name(Tag::constructorTag) << std::endl;
		Tag *obj = new Tag(Tag::constructorTag);
		Tag::constructorTag = NULL;
		obj->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
		std::cout << "New hello ConstructCall 2" << std::endl;
	} else {
		std::cout << "New hello not ConstructCall 1" << std::endl;
		const int argc = 1;
		v8::Local<v8::Value> argv[argc] = {info[0]};
		v8::Local<v8::Function> cons = Nan::New(constructor());
		info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
		std::cout << "New hello not ConstructCall 2" << std::endl;
	}
}

v8::Handle<v8::Value> Tag::Instantiate(MifareTag constructorTag) {
	std::cout << "instanciate hello 1" << std::endl;
	Nan::HandleScope scope;

	Tag::constructorTag = constructorTag;
	std::cout << "instanciate hello tagname:" << freefare_get_tag_friendly_name(constructorTag) << std::endl;
	v8::Local<v8::Value> argv[0] = {};

	std::cout << "instanciate hello 2" << std::endl;
	v8::Local<v8::Function> cons = Nan::New(constructor());
	std::cout << "instanciate hello 3" << std::endl;
	v8::Handle<v8::Value> rtn = Nan::NewInstance(cons, 0, argv).ToLocalChecked();
	std::cout << "instanciate hello 4" << std::endl;
	v8::String::Utf8Value tmp(Nan::ToDetailString(rtn).ToLocalChecked());
	std::cout << "instanciate hello 5:" << std::string(*tmp) << std::endl;
	return rtn;
}

NAN_METHOD(Tag::GetTagType) {
	Tag* obj = ObjectWrap::Unwrap<Tag>(info.This());

	enum mifare_tag_type type = freefare_get_tag_type(obj->tag);
	std::string typeStr = "Unknown";
	switch (type) {
		case CLASSIC_1K: typeStr = "MIFARE_CLASSIC_1K"; break;
		case CLASSIC_4K: typeStr = "MIFARE_CLASSIC_4K"; break;
		case DESFIRE: typeStr = "MIFARE_DESFIRE"; break;
		case ULTRALIGHT: typeStr = "MIFARE_ULTRALIGHT"; break;
		case ULTRALIGHT_C: typeStr = "MIFARE_ULTRALIGHT_C"; break;
	}

	info.GetReturnValue().Set(Nan::New<v8::String>(typeStr).ToLocalChecked());
}

NAN_METHOD(Tag::GetTagFriendlyName) {
	Tag* obj = ObjectWrap::Unwrap<Tag>(info.This());

	const char* friendlyName = freefare_get_tag_friendly_name(obj->tag);

	info.GetReturnValue().Set(Nan::New<v8::String>(friendlyName).ToLocalChecked());
}


NAN_METHOD(Tag::GetTagUID) {
	Tag* obj = ObjectWrap::Unwrap<Tag>(info.This());

	char* uid = freefare_get_tag_uid(obj->tag);

	info.GetReturnValue().Set(Nan::New<v8::String>(uid).ToLocalChecked());

	free(uid);
}
