extern	int iscaptured(int g,int maxply,int maxnodes,int maxlibs,int tm,int ldrno);
extern	void defonelib(int ply,int starts,int g,int tm,int gs);
extern	int defatari(int ply,int g,int tm,int gs,int lastmove);
extern	int def_atk_nbr(int gs,int grl,int mvp,int g,int libamt,int grllibs,int tm);
extern	int jump_to_escape(int gs,int mvp);
extern	int play_next_to_group(int gs,int mvp,int g,int el);
extern	int getefflibs(int g,int n,int ldrno);
extern	int attack2libs(int gs,int g,int *mvp,int tm);
extern	void gendefmoves(int ply,int starts,int g,int tm,int ldrno);
extern	int def_two_stone_wall(int l1,int l2,int gs,int mvptr);
extern	int jump(int lib,int g);
extern	void deftwolibs(int ply,int starts,int g,int tm,int gs);
extern	int atktwolibs(int ply,int starts,int g,int tm,int gs);
extern	void genatkmoves(int ply,int starts,int g,int tm,int ldrno);
extern	void genrestatk(int ply,int starts,int g,int tm,int gs,int ldrno);
extern	int livesordies(int starts,int maxlibs,int tm,int color,int ply,int ldrno);