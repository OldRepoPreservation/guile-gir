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

(use-modules (gir))

(define repo (g-irepository-get-default))
(define namespace "Everything")
(g-irepository-require repo namespace)

(define (call func-name args)
    (display "Calling '")
    (display func-name)
    (display "'..")
    (let ((func (g-irepository-find-by-name repo namespace func-name)))
        (let ((out-args (call-with-values
                              (lambda () (g-function-info-invoke func args))
                                         list)))
             (display "DONE.\n")
             out-args)))

;; UTF-8 related functions

;; functions dealing with constant strings
(define (test-utf8-const-in str)
        (call "test_utf8_const_in" (list str)))

(define (test-utf8-const-return)
        (call "test_utf8_const_return" '()))

;; functions dealing with non-constant strings
(define (test-utf8-nonconst-in str)
        (call "test_utf8_nonconst_in" (list str)))

(define (test-utf8-nonconst-return)
        (call "test_utf8_nonconst_return" '()))

(define (test-utf8-out)
        (display (call "test_utf8_out" '())))

;; functions dealing with complex types
(define (test-simple-boxed-a-const-return)
        (display (call "test_simple_boxed_a_const_return" '())))

; Now we test each toplevel static function
(test-utf8-const-in (car (test-utf8-const-return)))
(test-utf8-nonconst-in (car (test-utf8-nonconst-return)))
(test-utf8-out)
(test-simple-boxed-a-const-return)

