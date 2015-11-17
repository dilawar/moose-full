;;((nil . ((projectile-enable-caching . t))
;;      ((projectile-project-compilation-cmd "make develop"))))

;;http://stackoverflow.com/questions/24097839/how-to-add-include-path-to-flycheck-c-c-clang
;; (add-hook 'c++-mode-hook
;;           (lambda () (setq flycheck-clang-include-path
;;                            (list (expand-file-name "~/programming/moogli/moogli/bin/include/")))))

;;http://stackoverflow.com/questions/69934/set-4-space-indent-in-emacs-in-text-mode
((nil . ((projectile-project-compilation-cmd . "make build")
         (c++-mode (tab-width . 4)
                   (eval . (progn
                             (setq tab-stop-list (number-sequence 4 200 4))))))))

