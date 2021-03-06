;; Copyright (C) 2009 Free Software Foundation.
;;
;; Author: Zeeshan Ali (Khattak) <zeeshanak@gnome.org>
;;
;; This file is part of Guile-GIR.
;;
;; Guile-GIR is free software: you can redistribute it and/or modify it under
;; the terms of the GNU Lesser General Public License as published by the Free
;; Software Foundation, either version 3 of the License, or (at your option) any
;; later version.
;;
;; Guile-GIR is distributed in the hope that it will be useful, but WITHOUT ANY
;; WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
;; A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
;; details.
;;
;; You should have received a copy of the GNU Lesser General Public License
;; along with Guile-GIR.  If not, see <http://www.gnu.org/licenses/>.

(define-module (gir)
        :export (g-irepository-get-default
                 g-irepository-require
                 g-irepository-find-by-name
                 g-registered-type-info-get-g-type
                 g-object-info-get-type-name
                 g-object-info-get-type-init
                 g-object-info-get-abstract
                 g-object-info-get-parent
                 g-object-info-get-n-interfaces
                 g-object-info-get-interface
                 g-object-info-get-n-fields
                 g-object-info-get-field
                 g-object-info-get-n-properties
                 g-object-info-get-property
                 g-object-info-get-n-methods
                 g-object-info-get-method
                 g-object-info-find-method
                 g-object-info-get-n-signals
                 g-object-info-get-signal
                 g-object-info-get-n-vfuncs
                 g-object-info-get-vfunc
                 g-object-info-get-n-constants
                 g-object-info-get-constant
                 g-object-info-get-class-struct
                 g-callable-info-get-return-type
                 g-callable-info-get-caller-owns
                 g-callable-info-may-return-null
                 g-callable-info-get-n-args
                 g-callable-info-get-arg
                 g-function-info-get-symbol
                 g-function-info-get-flags
                 g-function-info-get-property
                 g-function-info-get-vfunc
                 g-function-info-invoke
                 g-arg-info-get-direction
                 g-arg-info-is-dipper
                 g-arg-info-is-return-value
                 g-arg-info-is-optional
                 g-arg-info-may-be-null
                 g-arg-info-get-ownership-transfer
                 g-arg-info-get-scope
                 g-arg-info-get-closure
                 g-arg-info-get-destroy
                 g-arg-info-get-type
                 g-i-transfer-nothing
                 g-i-transfer-container
                 g-i-transfer-everything
                 g-i-function-is-method
                 g-i-function-is-constructor
                 g-i-function-is-getter
                 g-i-function-is-setter
                 g-i-function-wraps-vfunc
                 g-i-function-throws
                 g-i-direction-in
                 g-i-direction-out
                 g-i-direction-inout
                 g-i-scope-type-invalid
                 g-i-scope-type-call
                 g-i-scope-type-async
                 g-i-scope-type-notified))

(dynamic-call "init"
              (dynamic-link "libguile-gir-1.0.so"))
