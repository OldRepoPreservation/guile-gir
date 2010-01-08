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

(define test-obj-info (g-irepository-find-by-name repo namespace "TestObj"))
(assert (not (unspecified? test-obj-info)))

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

(define (method-call obj-info method-name args)
  (let ((method (g-object-info-find-method obj-info method-name)))
    (call method args)))

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

;; Douglas Adams is a great writer but he got the number wrong. It's 47, not 42.
(define (test-callback)
  (let* ((meaning-of-life 47)
         (number (car (simple-call "test_callback"
                                   (list (lambda () meaning-of-life))))))
    (assert (equal? number meaning-of-life))))

;; user-data is useless in Scheme since we have closures at our disposal but
;; lets use it, just for the sake of completeness.
(define (test-callback-destroy-notify)
  (let* ((meaning-of-life 47)
         (number (car (simple-call "test_callback_destroy_notify"
                                   (list (lambda (user-data) meaning-of-life)
                                         #f
                                         display)))))
    (assert (equal? number meaning-of-life))))

;; Methods
(define (test-obj-static-method)
  (method-call test-obj-info "static_method" (list 47)))

(define (test-obj-new-from-file)
  (car (method-call test-obj-info "new_from_file" (list "hi"))))

(define (test-obj-do-matrix test-obj)
  (method-call test-obj-info "do_matrix" (list test-obj "hello")))

(define (test-obj-instance-method test-obj)
  (method-call test-obj-info "instance_method" (list test-obj)))

(define (test-obj-set-bare test-obj another-obj)
  (method-call test-obj-info "set_bare" (list test-obj another-obj)))

;; Now we test each toplevel static function
(test-utf8-const-in (car (test-utf8-const-return)))
(test-utf8-nonconst-in (car (test-utf8-nonconst-return)))
(test-utf8-out)
(test-simple-boxed-a-const-return)

(test-callback)
(test-callback-destroy-notify)

;; First test TestObj methods
(test-obj-static-method)
(let ((test-obj (test-obj-new-from-file)))
  (test-obj-do-matrix test-obj)
  (test-obj-instance-method test-obj)
  (let ((another-obj (test-obj-new-from-file)))
    (test-obj-set-bare test-obj another-obj)))

