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

;; Test cases against 'Everything' module provided by GIR.

;; Exits the script if form evaluates to #f
(define-macro (assert x)
              `(if (not ,x) (error "assertion failed" ',x)))

(use-modules (gir))

(define repo (g-irepository-get-default))
(define namespace "Everything")
(g-irepository-require repo namespace)

(define (call func args)
    (display "Calling '")
    (display (g-function-info-get-symbol func))
    (display "'..")
    (let ((out-args (call-with-values (lambda () (g-function-info-invoke func
                                                                         args))
                                      list)))
         (display "DONE.\n")
         out-args))

(define (simple-call func-name args)
    (let ((func (g-irepository-find-by-name repo namespace func-name)))
         (call func args)))

;; UTF-8 related functions

;; functions dealing with constant strings
(define (test-utf8-const-in str)
        (simple-call "test_utf8_const_in" (list str)))

(define (test-utf8-const-return)
        (simple-call "test_utf8_const_return" '()))

;; functions dealing with non-constant strings
(define (test-utf8-nonconst-in str)
        (simple-call "test_utf8_nonconst_in" (list str)))

(define (test-utf8-nonconst-return)
        (simple-call "test_utf8_nonconst_return" '()))

(define (test-utf8-out)
        (simple-call "test_utf8_out" '()))

;; functions dealing with complex types
(define (test-simple-boxed-a-const-return)
        (simple-call "test_simple_boxed_a_const_return" '()))

; Now we test each toplevel static function
(test-utf8-const-in (car (test-utf8-const-return)))
(test-utf8-nonconst-in (car (test-utf8-nonconst-return)))
(test-utf8-out)
(test-simple-boxed-a-const-return)

