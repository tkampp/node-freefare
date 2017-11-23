#include "tag.h"

using namespace Nan;


class ntag21x_connectWorker : public AsyncWorker {
public:
	ntag21x_connectWorker(Callback *callback, MifareTag tag)
	: AsyncWorker(callback), tag(tag), error(0) {}
	~ntag21x_connectWorker() {}

	void Execute () {
		error = ntag21x_connect(tag);

		// TODO: implement error handling
		ntag21x_get_info(tag);
	}

	void HandleOKCallback () {
		Nan::HandleScope scope;

		v8::Local<v8::Value> argv[] = {
			New<v8::Number>(error)
		};

		callback->Call(1, argv);
	}
private:

	// Our current tag
	MifareTag tag;

	// Error ID or 0
	int error;

};
NAN_METHOD(Tag::ntag21x_connect) {
	Tag* obj = ObjectWrap::Unwrap<Tag>(info.This());
	Callback *callback = new Callback(info[0].As<v8::Function>());
	AsyncQueueWorker(new ntag21x_connectWorker(callback, obj->tag));
}


class ntag21x_disconnectWorker : public AsyncWorker {
public:
	ntag21x_disconnectWorker(Callback *callback, MifareTag tag)
	: AsyncWorker(callback), tag(tag), error(0) {}
	~ntag21x_disconnectWorker() {}

	void Execute () {
		error = ntag21x_disconnect(tag);
	}

	void HandleOKCallback () {
		Nan::HandleScope scope;

		v8::Local<v8::Value> argv[] = {
			New<v8::Number>(error)
		};

		callback->Call(1, argv);
	}
private:

	// Our current tag
	MifareTag tag;

	// Error ID or 0
	int error;

};
NAN_METHOD(Tag::ntag21x_disconnect) {
	Tag* obj = ObjectWrap::Unwrap<Tag>(info.This());
	Callback *callback = new Callback(info[0].As<v8::Function>());
	AsyncQueueWorker(new ntag21x_disconnectWorker(callback, obj->tag));
}


class ntag21x_readWorker : public AsyncWorker {
public:
	ntag21x_readWorker(Callback *callback, MifareTag tag, uint8_t page)
	: AsyncWorker(callback), tag(tag), page(page), error(0) {}
	~ntag21x_readWorker() {}

	void Execute () {
		error = ntag21x_read4(tag, page, &data[0]);
	}

	void HandleOKCallback () {
		Nan::HandleScope scope;

		Nan::MaybeLocal<v8::Object> buf =  Nan::CopyBuffer(reinterpret_cast<char*>(data), sizeof(data));

		v8::Local<v8::Value> argv[] = {
			New<v8::Number>(error),
			buf.ToLocalChecked()
		};

		callback->Call(2, argv);
	}
private:

	// Our current tag
	MifareTag tag;

	// Page to read
	uint8_t page;

	// Page content
	uint8_t data[4];

	// Error ID or 0
	int error;

};
NAN_METHOD(Tag::ntag21x_read4) {
	Tag* obj = ObjectWrap::Unwrap<Tag>(info.This());
	Callback *callback = new Callback(info[1].As<v8::Function>());
	AsyncQueueWorker(new ntag21x_readWorker(callback, obj->tag, info[0]->Uint32Value()));
}

// ntag21x_fast_read

class ntag21x_fastReadWorker : public AsyncWorker {
public:
	ntag21x_fastReadWorker(Callback *callback, MifareTag tag, uint8_t start_page, uint8_t end_page)
	: AsyncWorker(callback), tag(tag), start_page(start_page), end_page(end_page), error(0) {}
	~ntag21x_fastReadWorker() {}

	void Execute () {
		// avoid this for now
		if(start_page > end_page) {			
			end_page = start_page;
		}				
		
		int no_pages = end_page - start_page + 1;
		length = no_pages * 4;
		
		data = (uint8_t*) malloc(length*sizeof(uint8_t));
		error = ntag21x_fast_read(tag, start_page, end_page, data);
	}

	void HandleOKCallback () {
		Nan::HandleScope scope;

		Nan::MaybeLocal<v8::Object> buf =  Nan::CopyBuffer(reinterpret_cast<char*>(data), length);	
	
		v8::Local<v8::Value> argv[] = {
			New<v8::Number>(error),
			buf.ToLocalChecked()
		};

		free(data);

		callback->Call(2, argv);
	}
private:

	// Our current tag
	MifareTag tag;

	// Page to read
	uint8_t start_page;

	// Page to read
	uint8_t end_page;

	// Page content
	uint8_t* data;

	// Length
	size_t length;
	
	// Error ID or 0
	int error;

};
NAN_METHOD(Tag::ntag21x_fast_read) {
	Tag* obj = ObjectWrap::Unwrap<Tag>(info.This());	
	Callback *callback = new Callback(info[2].As<v8::Function>());
	AsyncQueueWorker(new ntag21x_fastReadWorker(callback, obj->tag, info[0]->Uint32Value(), info[1]->Uint32Value()));
}


class ntag21x_writeWorker : public AsyncWorker {
public:
	ntag21x_writeWorker(Callback *callback, MifareTag tag, uint8_t page, uint8_t data[4])
	: AsyncWorker(callback), tag(tag), page(page), error(0) {
		memcpy(this->data, data, sizeof(this->data));
	}
	~ntag21x_writeWorker() {}

	void Execute () {
		error = ntag21x_write(tag, page, data);
	}

	void HandleOKCallback () {
		Nan::HandleScope scope;

		v8::Local<v8::Value> argv[] = {
			New<v8::Number>(error)
		};

		callback->Call(1, argv);
	}
private:

	// Our current tag
	MifareTag tag;

	// Page to read
	uint8_t page;

	// Write content
	uint8_t data[4];

	// Error ID or 0
	int error;

};
NAN_METHOD(Tag::ntag21x_write) {
	Tag* obj = ObjectWrap::Unwrap<Tag>(info.This());
	Callback *callback = new Callback(info[2].As<v8::Function>());
	AsyncQueueWorker(new ntag21x_writeWorker(callback, obj->tag, info[0]->Uint32Value(), reinterpret_cast<unsigned char*>(node::Buffer::Data(info[1]))));
}

class ntag21x_getSubTypeWorker : public AsyncWorker {
public:
	ntag21x_getSubTypeWorker(Callback *callback, MifareTag tag)
	: AsyncWorker(callback), tag(tag), error(0) {}

	~ntag21x_getSubTypeWorker() {}

	void Execute () {
		ntag_tag_subtype st = ntag21x_get_subtype(tag);
		switch(st) {
			case ntag_tag_subtype::NTAG_213: 
				subtype = 213;
			    break;
			case ntag_tag_subtype::NTAG_215: 
				subtype = 215;
			    break;	
			case ntag_tag_subtype::NTAG_216:
				subtype = 216;
			    break;
			default:
				subtype = 0;
				break;				
		}
	}

	void HandleOKCallback () {
		Nan::HandleScope scope;

		v8::Local<v8::Value> argv[] = {
			New<v8::Number>(error),
            New<v8::Number>(subtype) 
		};

		callback->Call(2, argv);
	}
private:

	// Our current tag
	MifareTag tag;

	// Subtype
	int subtype;

	// Error ID or 0
	int error;

};
NAN_METHOD(Tag::ntag21x_get_subtype) {
	Tag* obj = ObjectWrap::Unwrap<Tag>(info.This());
	Callback *callback = new Callback(info[0].As<v8::Function>());
	AsyncQueueWorker(new ntag21x_getSubTypeWorker(callback, obj->tag));
}

