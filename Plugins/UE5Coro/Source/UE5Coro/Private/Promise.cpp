// Copyright © Laura Andelare
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the disclaimer
// below) provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
// 
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
// THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
// CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
// NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "UE5Coro/AsyncCoroutine.h"

using namespace UE5Coro::Private;

#if UE5CORO_DEBUG
std::atomic<int> FPromise::LastDebugID = -1; // -1 = no coroutines yet
// This is a synchronous call stack that doesn't follow or track co_await!
thread_local TArray<FPromise*> FPromise::ResumeStack;
#endif

FPromise::FPromise(const TCHAR* PromiseType)
#if UE5CORO_DEBUG
	: DebugID(++LastDebugID), DebugPromiseType(PromiseType)
#endif
{
}

FPromise::~FPromise()
{
#if UE5CORO_DEBUG
	checkf(Alive == Expected, TEXT("Double coroutine destruction"));
	Alive = 0;
#endif
	Continuations.Broadcast();
}

void FPromise::CheckAlive()
{
#if UE5CORO_DEBUG
	// Best effort but ultimately unreliable check for stale objects
	checkf(Alive == Expected,
	       TEXT("Attempted to access or await a destroyed coroutine"));
#endif
}

void FPromise::Resume()
{
	CheckAlive();
#if UE5CORO_DEBUG
	checkf(ResumeStack.Num() == 0 || ResumeStack.Last() != this,
	       TEXT("Internal error"));
	ResumeStack.Push(this);
#endif
}

void FPromise::EndResume()
{
	// Coroutine resumption might result in `this` having been freed already and
	// not being considered `Alive`. This is technically undefined behavior.
#if UE5CORO_DEBUG
	checkf(ResumeStack.Last() == this, TEXT("Internal error"));
	ResumeStack.Pop();
#endif
}

void FPromise::unhandled_exception()
{
#if PLATFORM_EXCEPTIONS_DISABLED
	check(!"Exceptions are not supported");
#else
	throw;
#endif
}

TMulticastDelegate<void()>& FPromise::OnCompletion()
{
	CheckAlive();
	return Continuations;
}

FAsyncCoroutine FPromise::get_return_object()
{
	return FAsyncCoroutine(FHandle::from_promise(*this));
}
