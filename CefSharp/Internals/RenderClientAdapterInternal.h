// Copyright � 2010-2013 The CefSharp Project. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

#pragma once

#include "Stdafx.h"
#include "ClientAdapter.h"

namespace CefSharp
{
    interface class IRenderWebBrowser;

    namespace Internals
    {
        using namespace System;

        private class RenderClientAdapterInternal : public ClientAdapter,
            public CefRenderHandler
        {
        private:
            gcroot<IRenderWebBrowser^> _renderBrowserControl;

        public:
            RenderClientAdapterInternal(IRenderWebBrowser^ offscreenBrowserControl) :
                ClientAdapter((IWebBrowser^) offscreenBrowserControl)
            {
                _renderBrowserControl = offscreenBrowserControl;
            }

            ~RenderClientAdapterInternal() { _renderBrowserControl = nullptr; }

            // CefClient
            virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE { return this; }

            // CefRenderHandler
            virtual DECL bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) OVERRIDE
            {
                // TODO: add a real implementation... :)
                rect = CefRect(0, 0, 500, 500);
                return true;
            }

            virtual DECL void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE
            {
                _renderBrowserControl->SetPopupIsOpen(show);
            }

            virtual DECL void OnPopupSize(CefRefPtr<CefBrowser> browser, const CefRect& rect) OVERRIDE
            {
                _renderBrowserControl->SetPopupSizeAndPosition((IntPtr) (void*) &rect);
            }

            virtual DECL void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects,
                const void* buffer, int width, int height) OVERRIDE
            {
                auto intPtrBuffer = (IntPtr) (void *) buffer;

                if (type == PET_VIEW)
                {
                    _renderBrowserControl->SetBuffer(width, height, intPtrBuffer);
                }
                else if (type == PET_POPUP)
                {
                    _renderBrowserControl->SetPopupBuffer(width, height, intPtrBuffer);
                }
            }

            virtual DECL void OnCursorChange(CefRefPtr<CefBrowser> browser, CefCursorHandle cursor) OVERRIDE
            {
                _renderBrowserControl->SetCursor((IntPtr) cursor);
            }

            IMPLEMENT_REFCOUNTING(RenderClientAdapterInternal)
        };
    }
}