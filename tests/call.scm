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

;;
;; A simple guile script to execute function calls from GIR Libraries.
;;
;; Usage: guile -s call.scm NAMESPACE FUNCTION [ARG1 ..]
;;
;; Example:
;;
;; $ guile -s call.scm GLib filename_to_uri /home/zeenix localhost
;;

(define (call repo namespace func args)
    (call-with-values (lambda ()
                              (g-function-info-invoke
                                        (g-irepository-find-by-name repo
                                                                    namespace
                                                                    func)
                                        args))
                       list))

(use-modules (gir))
(let ((repo (g-irepository-get-default))
      (namespace (list-ref (command-line) 1)))
     (g-irepository-require repo namespace)
     (display (call repo
                    namespace
                    (list-ref (command-line) 2)
                    (list-tail (command-line) 3))))
