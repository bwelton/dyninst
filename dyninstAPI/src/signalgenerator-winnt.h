/*
 * Copyright (c) 1996-2004 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * This license is for research uses.  For such uses, there is no
 * charge. We define "research use" to mean you may freely use it
 * inside your organization for whatever purposes you see fit. But you
 * may not re-distribute Paradyn or parts of Paradyn, in any form
 * source or binary (including derivatives), electronic or otherwise,
 * to any other organization or entity without our permission.
 * 
 * (for other uses, please contact us at paradyn@cs.wisc.edu)
 * 
 * All warranties, including without limitation, any warranty of
 * merchantability or fitness for a particular purpose, are hereby
 * excluded.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * Even if advised of the possibility of such damages, under no
 * circumstances shall we (or any other person or entity with
 * proprietary rights in the software licensed hereunder) be liable
 * to you or any third party for direct, indirect, or consequential
 * damages of any character regardless of type of action, including,
 * without limitation, loss of profits, loss of use, loss of good
 * will, or computer failure or malfunction.  You agree to indemnify
 * us (and any other person or entity with proprietary rights in the
 * software licensed hereunder) for any and all liability it may
 * incur to third parties resulting from your use of Paradyn.
 */

/* $Id: signalgenerator-winnt.h,v 1.1 2006/03/29 21:38:37 bernat Exp $
 */

#ifndef _SIGNAL_GENERATOR_WINNT_H
#define _SIGNAL_GENERATOR_WINNT_H

// Normally included automatically in signalgenerator.h

///////////////////////////////////////////////////////////////////
////////// Handler section
///////////////////////////////////////////////////////////////////

// These are the prototypes for the NT-style signal handler
// NT has a two-level continue mechanism which we have to handle.
// There is a process-wide debug pause/resume, and a per-thread flag
// to pause or resume. We use the per-thread for pause() and continueProc(),
// and the process-wide for debug events. The process is stopped when
// the signal handler executes, and is re-run at the end. If the process
// should stay stopped the handlers must pause it explicitly.


class SignalGenerator : public SignalGeneratorCommon
{
  friend class SignalHandler;
  friend class SignalGeneratorCommon;
  friend class process;

  public:
   virtual ~SignalGenerator() {}

   HANDLE getProcessHandle() {return (HANDLE)procHandle;}
   HANDLE getThreadHandle() {return (HANDLE)thrHandle;}
  SignalGenerator(char *idstr, pdstring file, pdstring dir,
                         pdvector<pdstring> *argv,
                         pdvector<pdstring> *envp,
                         pdstring inputFile,
                         pdstring outputFile,
                         int stdin_fd, int stdout_fd,
                         int stderr_fd)
    : SignalGeneratorCommon(idstr, file, dir, argv, envp, inputFile, outputFile, 
                      stdin_fd, stdout_fd, stderr_fd),
      procHandle(-1), thrHandle(-1) {}

  SignalGenerator(char *idstr, pdstring file, int pid)
    : SignalGeneratorCommon(idstr, file, pid),
      procHandle(-1), thrHandle(-1) {} 

   bool waitingForStop() {return false;}
   void setWaitingForStop(bool flag) {;}

  private:
  SignalHandler *newSignalHandler(char *name, int id);
  virtual bool forkNewProcess();
  virtual bool attachProcess();
  virtual bool waitForStopInline();
  bool waitNextEventInternal(EventRecord &);

  bool decodeEvent(EventRecord &);
  bool decodeBreakpoint(EventRecord &);
  bool decodeException(EventRecord &);

  int procHandle;
  int thrHandle;
};

#endif