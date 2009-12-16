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

(define (call repo namespace func args)
    (call-with-values (lambda () (g-function-info-invoke func args))
                       list))

(define (test-utf8-out repo namespace)
    (let ((func (g-irepository-find-by-name repo namespace "test_utf8_out")))
        (display (call repo namespace func '()))))

(define (test-simple-boxed-a-const-return repo namespace)
    (let ((func (g-irepository-find-by-name
                                        repo
                                        namespace
                                        "test_simple_boxed_a_const_return")))
        (display (call repo namespace func '()))))

(use-modules (gir))
(let ((repo (g-irepository-get-default))
      (namespace "Everything"))
     (g-irepository-require repo namespace)

     ; Now we test each toplevel static function
     (test-utf8-out repo namespace)
     (test-simple-boxed-a-const-return repo namespace))

