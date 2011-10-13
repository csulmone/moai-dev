// Copyright (c) 2011 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXAMPLES_GETURL_GETURL_HANDLER_H_
#define EXAMPLES_GETURL_GETURL_HANDLER_H_

#include <string>
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/url_response_info.h"
#include "ppapi/cpp/url_request_info.h"
#include "ppapi/cpp/instance.h"


// GetURLHandler is used to download data from |url|. When download is
// finished or when an error occurs, it posts a message back to the browser
// with the results encoded in the message as a string and self-destroys.
//
// EXAMPLE USAGE:
// GetURLHandler* handler* = GetURLHandler::Create(instance,url);
// handler->Start();

class NaClFile;

typedef void (* GetURLCallback )( void *instance, const char *buffer, int32_t size );

class GetURLHandler {
 public:

	 enum {
		 GET,
		 HEAD,
	 };

	static GetURLHandler* Create ( pp::Instance* instance_, const std::string& url );

	bool Start ( GetURLCallback callback, NaClFile *file );

	void SetMethod ( int method );

 private:
	static const int kBufferSize = 4096;

	GetURLHandler ( pp::Instance* instance_, const std::string& url );
	~GetURLHandler();

	// Callback fo the pp::URLLoader::Open().
	// Called by pp::URLLoader when response headers are received or when an
	// error occurs (in response to the call of pp::URLLoader::Open()).
	// Look at <ppapi/c/ppb_url_loader.h> and
	// <ppapi/cpp/url_loader.h> for more information about pp::URLLoader.
	void OnOpen(int32_t result);

	// Callback fo the pp::URLLoader::ReadResponseBody().
	// |result| contains the number of bytes read or an error code.
	// Appends data from this->buffer_ to this->url_response_body_.
	void OnRead(int32_t result);

	// Reads the response body (asynchronously) into this->buffer_.
	// OnRead() will be called when bytes are received or when an error occurs.
	void ReadBody();

	// Post a message back to the browser with the download results.
	void ReportResult(const std::string& fname,
					const std::string& text,
					bool success);

	// Post a message back to the browser with the download results and
	// self-destroy.  |this| is no longer valid when this method returns.
	void ReportResultAndDie(const std::string& fname,
							const std::string& text,
							bool success);

	GetURLCallback mCallback;

	NaClFile * mFile;

	pp::Instance* mInstance;  // Weak pointer.

	std::string url_;  // URL to be downloaded.

	pp::URLRequestInfo url_request_;
	pp::URLLoader mUrlLoader;  // URLLoader provides an API to download URLs.

	char buffer_[kBufferSize];  // buffer for pp::URLLoader::ReadResponseBody().
	int mMethod;

	std::string url_response_body_;  // Contains downloaded data.
	pp::CompletionCallbackFactory<GetURLHandler> cc_factory_;

	GetURLHandler(const GetURLHandler&);
	void operator=(const GetURLHandler&);
};

#endif  // EXAMPLES_GETURL_GETURL_HANDLER_H_

